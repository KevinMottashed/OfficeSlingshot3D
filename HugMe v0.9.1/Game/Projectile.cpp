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

cVector3d Projectile::getPosition() const
{
	return position;
}

cVector3d Projectile::getSpeed() const
{
	return speed;
}

void Projectile::setPosition(double x, double y, double z)
{
	position.x = x;
	position.y = y;
	position.z = z;
}

void Projectile::setSpeed(double x, double y, double z)
{
	speed.x = x;
	speed.y = y;
	speed.z = z;
}