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
	VirtualSlingshot(cWorld* world, const cVector3d& position);

	/**
	 * Destructor.
	 */
	~VirtualSlingshot(void);

	/**
	 * Retrieves the upper left corner of the slinshot object (left fork)
	 */
	cVector3d upperLeft();

	/**
	 * Retrieves the upper right corner of the slinshot object (right fork)
	 */
	cVector3d upperRight();

private:
	/**
	 * The slingshot's mesh object.
	 */
	cMesh* slingshotMesh;
};

#endif
