#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "al5d.h"

/** Test program.
  * moves shit about.
  */

typedef float radians;

/* Position is a function of how rotated the base is*/
int calcpos(radians baserotation, arm_state *state) {
  if(baserotation > M_PI) {
    baserotation = M_PI;
  }
  armSetRotation(state, JOINT_BASE, baserotation);
  armSetRotation(state, JOINT_WRIST, baserotation+(M_PI/2.0));
  return 0;
}

int main(int argc, char **argv) {
  arm_state *S = malloc(sizeof(arm_state));
  if(argc >= 2)
    armInit(S, "/dev/ttyUSB0");
  else
    armInit(S, NULL);

  for(int i = 0; i < NUM_JOINTS; ++i) {
    armSetSpeed(S, i, 1000);
    armSetRotation(S, i, M_PI/2);
  }
  for(float P = M_PI; P > 0; P -= 0.2) {
    calcpos(P, S);
    armFlush(S, 1);
    sleep(2);
  }
  for(float P = 0; P < M_PI; P += 0.2) {
    calcpos(P, S);
    armFlush(S, 1);
    sleep(2);
  }
}
