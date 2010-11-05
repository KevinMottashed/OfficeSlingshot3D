#include "VirtualBall.h"

VirtualBall::VirtualBall(cWorld* world, cODEWorld* ODEWorld)
{
	alreadyCollided = false;
	firstPullBack = true;

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
	firstPullBack = true;

	odeBall->setShowEnabled(true);

	// make the ball dynamic
	odeBall->enableDynamics();

	odeBall->setPosition(p.position());

	//Add a force just for show
	odeBall->addGlobalForceAtGlobalPos(p.force(), p.position());
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

void VirtualBall::reset()
{
	odeBall->setShowEnabled(false);
}

void VirtualBall::move(cVector3d newBallPos)
{
	if (firstPullBack) {
		cVector3d initPos = World::local_ball_starting_position;
		startingOffset = newBallPos - initPos;

		// Disable gravity for the ball
		odeBall->disableDynamics();

		odeBall->setPosition(initPos);

		// make the ball visible
		odeBall->setShowEnabled(true);
		firstPullBack = false;

	} else {
		odeBall->setPosition(newBallPos-startingOffset);
	}
}

cVector3d VirtualBall::calculateForceVector()
{
	cVector3d startPos = startingOffset;
	cVector3d endPos = (World::local_ball_starting_position + startingOffset) - odeBall->getPos();

	cVector3d force = endPos-startPos;
	
	force.x *= 200;
	force.y *= 500;
	force.z *= 1500;

	return force;
}