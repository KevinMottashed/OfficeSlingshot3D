#ifndef VIRTUAL_AVATAR_H
#define VIRTUAL_AVATAR_H

#include "chai3d.h"

class VirtualAvatar
{
private:
	cMesh* avatarMesh;
	cVector3d boundaryMin;
	cVector3d boundaryMax;

	bool isLocal;

public:
	VirtualAvatar(cWorld* world, cVector3d startingPosition, bool isLocal);
	~VirtualAvatar(void);

	void rotate(double ang);
	void translate(double ang);
	void updateBoundaries(double ang, cVector3d position);
	bool isInHitBox(cVector3d ballPos);
};

#endif