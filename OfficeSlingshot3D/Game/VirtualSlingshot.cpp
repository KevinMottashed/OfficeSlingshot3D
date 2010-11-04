#include "VirtualSlingshot.h"
#include "WorldConstants.h"

VirtualSlingshot::VirtualSlingshot(cWorld* world, const cVector3d& startingPosition)
{
	slingshotMesh = new cMesh(world);

	world->addChild(slingshotMesh);

	slingshotMesh->setPos(startingPosition);

	slingshotMesh->loadFromFile("Objects\\slingshot\\slingshot.obj");
	slingshotMesh->scale(2);

    // compute a boundary box
	slingshotMesh->computeBoundaryBox(true);

    // define some haptic friction properties
    slingshotMesh->setFriction(0.1f, 0.2f, true);

	slingshotMesh->setUseCulling(false, true);
}

VirtualSlingshot::~VirtualSlingshot(void)
{
}
