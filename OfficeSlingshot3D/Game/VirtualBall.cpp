#include "VirtualBall.h"

VirtualBall::VirtualBall(cWorld* world, cODEWorld* ODEWorld, bool _isLocal)
{
	alreadyCollided = false;
	firstPullBack = true;
	isLocal = _isLocal;

	ballMesh = new cMesh(world);

	ballMesh->loadFromFile("Objects\\ball\\ball.obj");
	ballMesh->scale(0.015f);

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

void VirtualBall::fire(const Projectile& p)
{
	alreadyCollided = false;
	firstPullBack = true;

	odeBall->setShowEnabled(true);

	// make the ball dynamic
	odeBall->enableDynamics();

	odeBall->setPosition(p.position());
	odeBall->setLinearVelocity(cVector3d(0,0,0));
	odeBall->setAngularVelocity(cVector3d(0,0,0));

	//Add a force just for show
	odeBall->addGlobalForceAtGlobalPos(p.force(), p.position());
}

bool VirtualBall::getAlreadyCollided() const
{
	return alreadyCollided;
}

void VirtualBall::collided()
{
	alreadyCollided = true;
}

cVector3d VirtualBall::getBallPos() const
{
	return odeBall->getPos();
}

cVector3d VirtualBall::getBallCenter() const
{
	cVector3d ballPos = odeBall->getPos();
	if(isLocal) {
		return cVector3d(ballPos.x, ballPos.y+0.18f, ballPos.z-5.1f);
	}
	else {
		return cVector3d(ballPos.x, ballPos.y+0.18f, ballPos.z+5.1f);
	}
}

void VirtualBall::reset()
{
	odeBall->setShowEnabled(false);

	alreadyCollided = false;
	firstPullBack = true;
}

void VirtualBall::move(const cVector3d& newBallPos)
{
	if (firstPullBack) {
		cVector3d initPos;
		if(isLocal) {
			initPos = World::local_ball_starting_position;
		} else {
			initPos = World::peer_ball_starting_position;
		}
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
	
	force.x *= 250.0f;
	force.y *= 500.0f;
	force.z *= 1500.0f;

	return force;
}