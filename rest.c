#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "al5d.h"

int main(int argc, char **argv) {
  arm_state *S = malloc(sizeof(arm_state));
  armInit(S, "/dev/ttyUSB0");

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
}
