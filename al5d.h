#define NUM_JOINTS 6

static int servo_range[] = 
	{ 590,	2450,
	  650,	2320,
	  640,	2100,
	  1300,	2500,
	  1000,	2200,
	  690,	2500
	};

static double angle_offset[] =
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
void armSetRotation(arm_state * as, int joint, double theta);
void armSetSpeed(arm_state * as, int joint, short speed);
void armFlush(arm_state * as);

