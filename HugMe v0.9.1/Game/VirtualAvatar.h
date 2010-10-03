#ifndef VIRTUAL_AVATAR_H
#define VIRTUAL_AVATAR_H

#include "stdafx.h"
#include "chai3d.h"

class VirtualAvatar
{
private:
	cVector3d minPos;
	cVector3d maxPos;

public:
	VirtualAvatar(cVector3d position);
	~VirtualAvatar(void);

	bool isColliding(cVector3d ballPostion);
	cVector3d getMinPos();
	cVector3d getMaxPos();
};

#endif