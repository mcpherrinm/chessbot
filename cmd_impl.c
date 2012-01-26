#include "al5d.h"
#include "cli.h"
#include <unistd.h>

arm_state s;
arm_state *S = &s;

int main(int argc, char **argv) {
  armInit(S, "/dev/ttyUSB0");
  cli();
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
  armSetRotation(S, JOINT_BASE, 2);
  armSetRotation(S, JOINT_TILT, 0);
  armSetRotation(S, JOINT_GRIP, M_PI);
  armFlush(S, 1);

  return 0;
}

int touch(char argc, char **argv) {
  for(int i = 0; i < NUM_JOINTS; ++i) {
    armSetSpeed(S, i, 200);
    armSetRotation(S, i, M_PI/2);
  }
  armFlush(S, 1);
  sleep(3);
  armSetRotation(S, JOINT_WRIST, M_PI/1.7);
  armSetRotation(S, JOINT_SHOULDER, M_PI/2.46);
  armFlush(S, 1);

  return 0;
}
