#include "targeting.h"

/*
 * aquiretarget takes an x-y position for the wrist and modifies a joint
 * position for the shoulder and elbow.
 *
 * The other joints will all be left unmodified.
 *
 * return value indicates success.
 */

bool aquiretarget(struct xy target, struct arm_position *result) {
  return false;
}

/*
 * pointat takes a desired gripper angle from vertical and updates an arm_position
 * based on the position the arm_position represents.
 *
 * Pointing at the ground (a common case!) is 0. Pointing to sky is Pi.
 *
 * Only the JOINT_WRIST variable is updated in arm_position, and only if the
 * return value is true.
 *
 * Returns true iff the desired angle is inside the possible restrictions.
 */

bool pointat(float griprotation, struct arm_postion *posn) {
  /* Implementation notes:
   *  whores.
   */
  return false;
}

/*
 * planmove takes a desired arm_position and returns an array of intermediate
 * positions that avoids intersecting an exclusion zone, which is a a function
 * that checks something or other.
 *
 * planMove returns an int indicating the length of the move sequence.  It is
 * negative if a plan cannot be found.
 *
 * This is probably important to automated operation, but for now, it is
 * unimplemented.
 */

int planmove() {
  return -1;
}
