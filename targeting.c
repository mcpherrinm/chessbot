#include "targeting.h"
#include <stdio.h>


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
  const double x = target.x;
  const double y = target.y;

  const double distance = hypot(x, y);
  const double safety = 0;
  /* Hard Constraint Check: Can we reach? With a bit of safety */
  if( distance + safety > (length[0] + length[1])  ) {
    puts("Can't reach");
    return false;
  }
  /* Hard Constraint Check: Can we reach that close? */
  if(distance < fabs(length[0] - length[1])) {
    puts("Too close to reach");
    return false;
  }

  /* Hard constraint Check: Don't allow negative y, to avoid table intersection  */
  if(y < 10) {
    puts("Y to small");
    return false;
  }
  /* Hard constraint check: Make sure |x| is big enough to not intersect base*/
  //if(x < 100) {
   // puts("x to small");
    /* Todo:  allow x to be negative, or anything if y is tall enough */
    //return false;
  //}

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

  /*Useful values: squares of things */
  const double l0s = length[0]*length[0];
  const double l1s = length[1]*length[1];
  const double dts = x*x + y*y;

  /* First, find the elbow point
   *
   * Elbow is at the intersection of two circles, one around (0, 0)
   * and the other at (x, y), with radii being length[0] and [1].
   */
  struct xy elbowpos;
  /* midpoint between the two circle intersection points */
  struct xy midpoint;
  /*distance origin -> midpoint */
  const double midp_dist = (l0s - l1s + dts)/(2.0 * distance);
  midpoint.x = x * midp_dist/distance;
  midpoint.y = y * midp_dist/distance;

  /* distance midpoint -> elbow */
  const double elbow_dist = sqrt(l0s - midp_dist*midp_dist);
  /* offsets to intersection */
  const double rx = -y * (elbow_dist/distance);
  const double ry = -x * (elbow_dist/distance);

  /* Elbow is now one of two possible points:
   *  (x + rx, y + ry) or (x - rx), (y - ry)
   * TODO: Can it actually be the second solution?
   */

  elbowpos.x = x + rx;
  elbowpos.y = y + ry;

  /*  So now we have three points, (0, 0), elbow, (x, y)
   *  trig me some points
   */
  const double shoulder = atan2(y, x);
  const double elbow = shoulder + atan2(elbowpos.y - y , x - elbowpos.x);

  /*Sanity checks on angles: Avoid illegal positions*/

  /* uh
  if(shoulder > ????) {
    return false;
  }*/

  /* restriction on y should prevent this*/
  if(shoulder < 0) {
    puts("Backwards shoulder disallowed");
    return false;
  }

  /* self-intersection -- TODO determine value better
  if(elbow > M_PI-0.3) {
    puts("self-intersection");
    return false;
  }*/

  /*Elbow doesn't go negative*/
  if(elbow < 0) {
    printf("Disallow negative elbow %f\n", elbow);
    return false;
  }

  /* All checks pass, write data and return */
  result->joint[JOINT_SHOULDER] = shoulder;
  result->joint[JOINT_ELBOW] = elbow;
  return true;
}

/* Get the (x, y) coordinates of the wrist based on the angles that the arm is
 * currently in. Except for numeric stability, this is the inverse of
 * aquiretarget.
 */
bool getcoords(struct arm_position pos, struct xy *result) {
  double shoulderangle = pos.joint[JOINT_SHOULDER];
  double elbowangle = pos.joint[JOINT_ELBOW];

  struct xy elbow;

  elbow.x = cos(shoulderangle)*length[0];
  elbow.y = sin(shoulderangle)*length[0];

  double elbowtovertical = shoulderangle - elbowangle;

  result->x = elbow.x + sin(elbowtovertical)*length[1];
  result->y = elbow.y + cos(elbowtovertical)*length[1];

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
