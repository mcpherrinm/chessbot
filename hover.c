#include "al5d.h"


int main(int argc, char **argv) {
  arm_state s;
  arm_state *S = &s; /*hobo*/
  armInit(S, "/dev/ttyUSB0");

  for(int i = 0; i < NUM_JOINTS; ++i) {
    armSetSpeed(S, i, 1000);
    armSetRotation(S, i, M_PI/2);
  }
  armFlush(S, 1);
}
