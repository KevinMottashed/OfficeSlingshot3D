// Projectile.cpp: implementation of the Projectile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Projectile.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Projectile::Projectile() :
	position(cVector3d(0,0,0)),
	speed(cVector3d(0,0,0))
{
}

Projectile::Projectile(cVector3d position, cVector3d speed) :
	position(position),
	speed(speed)
{
}

Projectile::~Projectile()
{
}
