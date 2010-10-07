#ifndef VIRTUAL_SLINGSHOT_H
#define VIRTUAL_SLINGSHOT_H

#include "chai3d.h"

class VirtualSlingshot
{
private:
	cMesh* slingshotMesh;

	/**
	 * This is the offset used when firing balls from the slingshot
	 * This offset makes the balls come out of the sling instead of the handle
	 */
	static const cVector3d slingshot_sling_offset;

public:
	VirtualSlingshot(cWorld* world, const cVector3d startingPosition);
	~VirtualSlingshot(void);

	void move(cVector3d position);
	cVector3d getBallPosition();
};

#endif
