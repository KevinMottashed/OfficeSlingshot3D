#include "VirtualSlingshot.h"
#include "WorldConstants.h"

const cVector3d VirtualSlingshot::slingshot_sling_offset = cVector3d(0, 1.0, 0.0f);

VirtualSlingshot::VirtualSlingshot(cWorld* world, const cVector3d startingPosition)
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

void VirtualSlingshot::move(cVector3d position)
{
	if (World::local_slingshot_bounding_box.contains(position))
	{
		slingshotMesh->setPos(position);
	}
	return;
}

cVector3d VirtualSlingshot::getBallPosition()
{
	return slingshotMesh->getPos() + slingshot_sling_offset;
}
