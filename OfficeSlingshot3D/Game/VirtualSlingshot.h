#ifndef VIRTUAL_SLINGSHOT_H
#define VIRTUAL_SLINGSHOT_H

#include "chai3d.h"

/**
 * Represents a slingshot in the 3d environment.
 */

class VirtualSlingshot
{
public:
	/**
	 * Constructor.
	 * @param world The world in which to add the slingshot
	 * @param startingPosition The slingshot's starting position in the world
	 */
	VirtualSlingshot(cWorld* world, const cVector3d& startingPosition, const cCollisionAABBBox& boundingBox);

	/**
	 * Destructor.
	 */
	~VirtualSlingshot(void);

	/**
	 * Moves the slingshot within the virtual environment.
	 * @param position The new position.
	 */
	void move(cVector3d position);

	/**
	 * Retrieve the ball's position within the virtual environment.
	 * @return the ball position.
	 */
	cVector3d getBallPosition();

	

private:
	/**
	 * The slingshot's mesh object.
	 */
	cMesh* slingshotMesh;

	/**
	 * This is the offset used when firing balls from the slingshot
	 * This offset makes the balls come out of the sling instead of the handle
	 */
	static const cVector3d slingshot_sling_offset;

	cCollisionAABBBox boundingBox;
};

#endif
