#include <stdbool.h>
#include "al5d.h"
/*
 *   Targetting system for robot
 *   Functions for planning motion of robot
 */

struct arm_position {
  float joint[NUM_JOINTS];
};

/* Units: mm */
struct xy {
  float x;
  float y;
};

/*
 * aquiretarget takes an x-y position for the wrist and modifies a joint
 * position for the shoulder and elbow.
 *
 * The other joints will all be left unmodified.
 *
 * return value indicates success.
 */

bool aquiretarget(struct xy target, struct arm_position *result);

/*inverse of above */
bool getcoords(struct arm_position pos, struct xy *result);

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

bool pointat(float griprotation, struct arm_position *posn);
