#include "targeting.h"
#include <stdio.h>
#include <stdlib.h>

/*
bool aquiretarget(struct xy target, struct arm_position *result);
*/

bool trystuff(float x, float y) {
  struct xy t = { x,y };
  struct arm_position r;
  for(int i = 0; i < 32; i++) {
    printf("(%.20f, %.20f)\n", t.x, t.y);
    if(!aquiretarget(t, &r)) {
      printf("Failed!\n");
      return false;
    }
    printf("Angles: %f, %f\n", r.joint[JOINT_SHOULDER], r.joint[JOINT_ELBOW]);
    struct xy wut = t;
    getcoords(r, &t);
    if (t.x == wut.x && t.y == wut.y){
    //  return true;
    }
  }

  return false;
}

int TEST(int argc, char **argv) {
if (argc < 3) {
  puts("fuck off dude");
  return 0;
}
  float x, y;
  x = atoi(argv[1]);
  y = atoi(argv[2]);
      if(!trystuff(x, y)) {
        puts("oh ffs, fuck off");
        return -1;
  }
  return 0;
}
