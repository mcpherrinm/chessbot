#include "targeting.h"
#include "stdio.h"

/*
bool aquiretarget(struct xy target, struct arm_position *result);
*/

/*
 * Run through a series of points, and calculate the amount of error */
int TEST() {
  struct xy t = { 180, 148 };
  struct arm_position r;
  for(int i = 0; i < 32; i++) {
    printf("Point: (%f, %f)\n", t.x, t.y);
    if(aquiretarget(t, &r)) {
      printf("Angle: %f %f\n", r.joint[JOINT_SHOULDER], r.joint[JOINT_ELBOW]);
    } else {
      printf("Failed!\n");
      return 1;
    }
    getcoords(r, &t);
  }

  return 0;
}
