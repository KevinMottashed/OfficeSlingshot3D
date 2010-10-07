#ifndef VIRTUAL_BALL_H
#define VIRTUAL_BALL_H

#include "stdafx.h"
#include "Projectile.h"
#include "chai3d.h"
#include "CODE.h"
#include "Projectile.h"

class VirtualBall
{
private:
	cMesh* ballMesh;
	cODEGenericBody* odeBall;

public:
	VirtualBall(cWorld* world, cODEWorld* ODEWorld);
	~VirtualBall(void);

	void fire(Projectile p);
	cVector3d getMeshPos();
};

#endif