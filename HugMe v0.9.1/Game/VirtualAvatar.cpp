#include "VirtualAvatar.h"

VirtualAvatar::VirtualAvatar(cVector3d position)
{
	minPos = cVector3d(position.x-0.4, position.y-0.4, position.z);
	maxPos = cVector3d(position.x+0.4, position.y+0.4, position.z+1.0);
}

VirtualAvatar::~VirtualAvatar(void)
{
}

bool VirtualAvatar::isColliding(cVector3d ballPosition)
{
	return false;
}

cVector3d VirtualAvatar::getMinPos()
{
	return minPos;
}

cVector3d VirtualAvatar::getMaxPos()
{
	return maxPos;
}
