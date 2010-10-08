#include "VirtualAvatar.h"

VirtualAvatar::VirtualAvatar(cWorld* world, cVector3d startingPosition, bool isLocal): 
	isLocal(isLocal)
{
	avatarMesh = new cMesh(world);

	world->addChild(avatarMesh);

	avatarMesh->loadFromFile("Objects\\avatar\\avatar.obj");
	avatarMesh->scale(0.045f);

	avatarMesh->setPos(startingPosition);

	if(!isLocal){
		avatarMesh->rotate(cVector3d(0.0f, 1.0f, 0.0f), cDegToRad(-90));
	} else {
		avatarMesh->rotate(cVector3d(0.0f, 1.0f, 0.0f), cDegToRad(90));
	}

	avatarMesh->setUseCulling(true, true);
}

VirtualAvatar::~VirtualAvatar(void)
{
}

void VirtualAvatar::rotate(double ang)
{
	//Reset rotation to upright
	avatarMesh->rotate(avatarMesh->getRot().inv());

	if(!isLocal){
		ang = -ang;
		avatarMesh->rotate(cVector3d(0.0f, 1.0f, 0.0f), cDegToRad(-90));
	} else {
		avatarMesh->rotate(cVector3d(0.0f, 1.0f, 0.0f), cDegToRad(90));
	}

	//Apply rotation
	avatarMesh->rotate(cVector3d(0.0f, 0.0f, 1.0f), cDegToRad(-ang));
}

void VirtualAvatar::translate(double ang)
{
	if(!isLocal) ang = -ang;

	cVector3d prevPos = avatarMesh->getPos();
	avatarMesh->setPos(ang/75, prevPos.y, prevPos.z);
}

void VirtualAvatar::updateBoundaries(double ang, cVector3d position)
{
	boundaryMin += cVector3d(ang/75, 0.0f, 0.0f);
	boundaryMax += cVector3d(ang/75, 0.0f, 0.0f);

	boundaryMin -= position;
	boundaryMax += position;
}

bool VirtualAvatar::isInHitBox(cVector3d ballPos)
{
	return (boundaryMin.x<=ballPos.x && boundaryMin.y<=ballPos.y && 
		boundaryMin.z<=ballPos.z && boundaryMax.x>ballPos.x && 
		boundaryMax.y>ballPos.y && boundaryMax.z>ballPos.z);
}