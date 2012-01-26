#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "al5d.h"

int main(int argc, char **argv) {
  arm_state *S = malloc(sizeof(arm_state));
  if(argc >= 2)
    armInit(S, "/dev/ttyUSB0");
  else
    armInit(S, NULL);

  for(int i = 0; i < NUM_JOINTS; ++i) {
    armSetSpeed(S, i, 200);
    armSetRotation(S, i, M_PI/2);
  }
  armFlush(S, 1);
  sleep(3);
  armSetRotation(S, JOINT_WRIST, M_PI/1.7);
  armSetRotation(S, JOINT_SHOULDER, M_PI/2.46);
  armFlush(S, 1);
}
