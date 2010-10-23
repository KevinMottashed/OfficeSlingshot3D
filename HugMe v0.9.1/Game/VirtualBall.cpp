#include "VirtualBall.h"

VirtualBall::VirtualBall(cWorld* world, cODEWorld* ODEWorld)
{
	alreadyCollided = false;

	ballMesh = new cMesh(world);

	ballMesh->loadFromFile("Objects\\ball\\ball.obj");
	ballMesh->scale(0.015f);

	ballMesh->createAABBCollisionDetector(1.01, true, false);

	odeBall = new cODEGenericBody(ODEWorld);
	odeBall->setImageModel(ballMesh);

	//Calculate boundaries of physical ball
	odeBall->createDynamicMesh(false);

	odeBall->setUseCulling(false, true);

	// hide the ball until fired
	odeBall->setShowEnabled(false);
}

VirtualBall::~VirtualBall(void)
{
}

void VirtualBall::fire(Projectile p)
{
	alreadyCollided = false;

	odeBall->setPosition(p.position());

	//Add a force just for show
	odeBall->addGlobalForceAtGlobalPos(p.force(), p.position());

	// make the ball visible
	odeBall->setShowEnabled(true);
}

bool VirtualBall::getAlreadyCollided()
{
	return alreadyCollided;
}

void VirtualBall::collided()
{
	alreadyCollided = true;
}

cVector3d VirtualBall::getMeshPos()
{
	return odeBall->getPos();
}