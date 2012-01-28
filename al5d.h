#define NUM_JOINTS 6
#include<math.h>

#define M_PI 3.141592653589793

enum jointname {
  JOINT_BASE = 0,
  JOINT_SHOULDER = 1,
  JOINT_ELBOW = 2,
  JOINT_WRIST = 3,
  JOINT_GRIP = 4,
  JOINT_TILT = 5
};

static const int servo_range[] =
	{
	  590,	2450,
	  650,	2320,
	  640,	2100,
	  800,	2500,
	  1000,	2200,
	  690,	2500
	};

// shoulder->elbow, elbow ->wrist, wrist -> tip of long grip
static const float length[3] = {146.0, 187.0, 130.0 };

static const double angle_offset[] =
	{ 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };

struct arm_state_t {
	double joint_pos[NUM_JOINTS];
	char updated[NUM_JOINTS];
	short joint_p[NUM_JOINTS];
	short joint_s[NUM_JOINTS];

	int fd;
};
typedef struct arm_state_t arm_state;

int armInit(arm_state * as, char * path);
void armClose(arm_state * as);
void armSetRotation(arm_state * as, enum jointname joint, double theta);
void armSetSpeed(arm_state * as, enum jointname joint, short speed);
void armFlush(arm_state * as);

