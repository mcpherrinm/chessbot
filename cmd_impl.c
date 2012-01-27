#include "al5d.h"
#include "cli.h"
#include <unistd.h>
#include <math.h>
#include <string.h>
#include <stdio.h>

arm_state s;
arm_state *S = &s;

int rest();

int main(int argc, char **argv) {
  if(argc == 2) {
    if(0 == strcmp(argv[1], "-")) {
      armInit(S, 0);
    } else {
      armInit(S, argv[1]);
    }
  } else {
    armInit(S, "/dev/ttyUSB0");
  }
  rest(0, 0);
  cli();
  rest(0, 0);
  armClose(S);
}

int rest(int argc, char **argv) {
  for(int i = 0; i < NUM_JOINTS; ++i)
    armSetSpeed(S, i, 200);

  /* Empiracally determined positions for rest spot.
   * Obviously will need to change when we get a button
   * to indicate rest, and extend arm
   */
  armSetRotation(S, JOINT_WRIST, 2.1);
  armSetRotation(S, JOINT_ELBOW, M_PI/0.95);
  armSetRotation(S, JOINT_SHOULDER, M_PI/0.95);
  armSetRotation(S, JOINT_BASE, 2.5);
  armSetRotation(S, JOINT_ROLL, 0);
  armSetRotation(S, JOINT_GRIP, M_PI);
  armFlush(S);

  return 0;
}

int touch(char argc, char **argv) {
  for(int i = 0; i < NUM_JOINTS; ++i) {
    armSetSpeed(S, i, 200);
    armSetRotation(S, i, M_PI/2);
  }
  armFlush(S);
  sleep(3);
  armSetRotation(S, JOINT_WRIST, M_PI/1.7);
  armSetRotation(S, JOINT_SHOULDER, M_PI/2.46);
  armFlush(S);

  return 0;
}

enum jointname jointmap(char *name) {
  if(0 == strcmp("base", name)) {
    return JOINT_BASE;
  } else if(0 == strcmp("shoulder", name)) {
    return JOINT_SHOULDER;
  } else if(0 == strcmp("elbow", name)) {
    return JOINT_ELBOW;
  } else if(0 == strcmp("wrist", name)) {
    return JOINT_WRIST;
  } else if(0 == strcmp("grip", name)) {
    return JOINT_GRIP;
  } else if(0 == strcmp("roll", name)) {
    return JOINT_ROLL;
  }
  return NUM_JOINTS + 1;
}

/*set SERVO ANGLE*/
int set(char argc, char **argv) {
  if(argc != 3) {
    printf("set SERVO ANGLE(radians)\n");
    return 0;
  }
  enum jointname joint = jointmap(argv[1]);
  if(joint > NUM_JOINTS) {
    return 0;
  }
  float angle;
  sscanf(argv[2], "%f", &angle);
  armSetRotation(S, joint, angle);
  armFlush(S);

  return 0;
}

int up() {
  /*base shoulder elbow wrist grip roll*/
  armSetAll(S, 1.57, 1.57, 0, 3.14, 3.14, 1.57);
  armFlush(S);
  return 0;
}
