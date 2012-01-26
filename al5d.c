/* AL5D Robot Arm state-based control library
   Marc Burns, 2011
    This library handles communication, calibration, and
    basic self-collision detection on the AL5D robot arm.
    Users may call armInit(), followed by several
    instances of armSet*, followed by armFlush(). Upon
    calling armFlush(), the arm shall proceed to the
    state described by calls to armSet*. */

#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <math.h>
#include <errno.h>
#include <string.h>
#include "al5d.h"

#define min(x, y) ( ((x) < (y)) ? (x) : (y) )
#define max(x, y) ( ((x) > (y)) ? (x) : (y) )

#define TTY_BAUDRATE B115200

int armInit(arm_state * as, char * path) {
	int i, j;
	if(!as)
		return 0;

	if(path) {
		as->fd = open(path, O_RDWR|O_NOCTTY);
		if(as->fd == -1) {
			switch(errno) {
			case EACCES:
				fprintf(stderr, "al5d: Could not open %s for read and write.\n", path);
				break;
			case EINVAL:
				fprintf(stderr, "al5d: Got EINVAL opening %s.\n", path);
				break;
			default:
				fprintf(stderr, "al5d: Something horrible happened opening %s.\n", path);
				break;
			}
			as->fd = 0;
			return 0;
		}
	} else {
		/* Default to stdin if path is null*/
		as->fd = 1;
	}

	struct termios fd_te;
	memset(&fd_te, 0, sizeof(struct termios));
	if(tcgetattr(as->fd, &fd_te)) {
		fprintf(stderr, "al5d: Call to tcgetattr() failed.\n");
		close(as->fd);
		as->fd = 0;
		return 0;
	}

	if(cfsetispeed(&fd_te, TTY_BAUDRATE) || cfsetospeed(&fd_te, TTY_BAUDRATE)) {
		fprintf(stderr, "al5d: Calls to cfset[io]speed() failed.\n");
		close(as->fd);
		as->fd = 0;
		return 0;
	}

	if(tcsetattr(as->fd, TCSANOW, &fd_te)) {
		fprintf(stderr, "al5d: Call to tcsetattr() failed.\n");
		close(as->fd);
		as->fd = 0;
		return 0;
	}

	for(i=0;i<NUM_JOINTS;++i) {
		as->updated[i] = 1;
		as->joint_pos[i] = 0.f;
		as->joint_p[i] = 0;
		as->joint_s[i] = 0;
	}

	return 1;
}

void armClose(arm_state * as) {
	if(as && (as->fd > 0)) {
		close(as->fd);
		as->fd = 0;
	} else {
		fprintf(stderr, "al5d: Attempt to armClose() invalid context.\n");
	}
}

void armSetRotation(arm_state * as, enum jointname joint, double theta) {
	if(!as || !as->fd) {
		fprintf(stderr, "al5d: armSetRotation called on invalid context.\n");
		return;
	}

	if(joint >= NUM_JOINTS) {
		fprintf(stderr, "al5d: There is no joint %d.\n", joint);
		return;
	}

	if(as->joint_pos[joint] == theta)
		return;

	as->joint_pos[joint] = theta;
	as->updated[joint] = 1;
}

void armSetSpeed(arm_state * as, enum jointname joint, short speed) {
	if(!as || !as->fd) {
		fprintf(stderr, "al5d: armSetSpeed called on invalid context.\n");
		return;
	}

	if(joint >= NUM_JOINTS) {
		fprintf(stderr, "al5d: There is no joint %d.\n", joint);
		return;
	}

	as->joint_s[joint] = speed;
}

void armFlush(arm_state * as, int debug) {
	int i;
	if(!as || !as->fd) {
		fprintf(stderr, "al5d: armFlush called on invalid context.\n");
		return;
	}

	// Compute joint_p
	for(i=0;i<NUM_JOINTS;++i) {
		as->joint_p[i] =
			servo_range[2*i] +
			((as->joint_pos[i] / (M_PI)) *
			(double)(servo_range[2*i+1] - servo_range[2*i]));
	}

	char c_bfr[1024];
	char * c_pos = c_bfr;

	for(i=0;i<NUM_JOINTS;++i) {
		if(as->updated[i]) {
			as->updated[i] = 0;
			c_pos += snprintf(c_pos, 1024 - (c_pos - c_bfr),
				"#%d P%d S%d", i, as->joint_p[i], as->joint_s[i]);
			if(i<(NUM_JOINTS-1))
				c_pos += snprintf(c_pos, 1024 - (c_pos - c_bfr), " ");
		}
	}

	c_pos += snprintf(c_pos, 1024 - (c_pos - c_bfr), "\r\n");

	if((c_pos - c_bfr) < 4)
		return;

	write(as->fd, c_bfr, (c_pos - c_bfr));
}

