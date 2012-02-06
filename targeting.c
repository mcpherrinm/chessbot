#include "targeting.h"


/*
 * aquiretarget takes an x-y position for the wrist and modifies a joint
 * position for the shoulder and elbow.
 *
 * The other joints will all be left unmodified.
 *
 * return value indicates success.
 *
 * result is unmodified in event of failure.
 */

bool aquiretarget(struct xy target, struct arm_position *result) {
  double x = target.x;
  double y = target.y;

  double distance = sqrt(x*x + y*y);
  /* Hard Constraint Check: Can we reach? With a bit of safety */
  if( distance + 20 > length[0] + length[1]  ) {
    return false;
  }
  /* Hard constraint Check: Don't allow negative y, to avoid table intersection  */
  if(y < 10) {
    return false;
  }
  /* Hard constraint check: Make sure |x| is big enough to not intersect base*/
  if(x < 100) {
    /* Todo:  allow x to be negative, or anything if y is tall enough */
    return false;
  }

  /* Calculate the angles:
   *
   *              Y  .
   *                / \
   *                 |
   *                 |
   *                 |       ,-- Elbow
   *                        /
   *                    __
   *                   |__|------=\  -- target point (x, y)
   *                   / /        \\
   *                  / /
   *  Shoulder --.   / /
   *                /_/
   *              __|_|___                      -----> X
   *              | BASE |
   *
   *
   *  Shoulder points to Y at Pi/2. Points to X at 0.
   *  Elbow is straight at 0. Bent at Pi/2.
   */

  const double l0s = length[0]*length[0];
  const double l1s = length[1]*length[1];
  const double dts = distance*distance;

  double shoulder =
    /*angle from level to target point*/
    tan(y/x) 
    /* Cosine law on triangle (base, elbow, target) */
    + acos( (l0s + dts - l1s) /(2*length[0]*distance));

  /* inner angle of the triangle - cosine law again */
  double elbowinner = acos((l0s + l1s - dts)/(2*length[0]*length[1]));

  /* elbow is the outer angle */
  double elbow = (M_PI/2) - elbowinner;

  /*Sanity checks on angles: */

  /* restriction on x should prevent this*/
  if(shoulder > M_PI/2) {
    return false;
  }

  /* restriction on y should prevent this*/
  if(shoulder < 0) return false;

  /* self-intersection -- TODO determine value better*/
  if(elbow > M_PI-0.3) return false;

  /*Elbow doesn't go negative*/
  if(elbow < 0) return false;

  /* All checks pass, write data and return */
  result->joint[JOINT_SHOULDER] = shoulder;
  result->joint[JOINT_ELBOW] = elbow;
  return true;
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

bool pointat(float griprotation, struct arm_position *posn) {
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
