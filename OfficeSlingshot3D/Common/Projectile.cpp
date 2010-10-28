#include "stdafx.h"
#include "Projectile.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Projectile::Projectile() :
	_position(cVector3d(0,0,0)),
	_force(cVector3d(0,0,0))
{
}

Projectile::Projectile(cVector3d position, cVector3d force) :
	_position(position),
	_force(force)
{
}

Projectile::~Projectile()
{
}

cVector3d Projectile::position() const
{
	return _position;
}

cVector3d Projectile::force() const
{
	return _force;
}

void Projectile::position(double x, double y, double z)
{
	_position.x = x;
	_position.y = y;
	_position.z = z;
}

void Projectile::position(const cVector3d& v)
{
	_position = v;
	return;
}

void Projectile::force(double x, double y, double z)
{
	_force.x = x;
	_force.y = y;
	_force.z = z;
}

void Projectile::force(const cVector3d& v)
{
	_force = v;
	return;
}

bool Projectile::equals(const Projectile& other, float epsilon)
{
	return	_position.equals(other.position(), epsilon) && 
			_force.equals(other.force(), epsilon);
}

std::ostream& operator<<(std::ostream& os, const Projectile& projectile)
{
	os << "position=" << projectile.position() << " force=" << projectile.force();
	return os;
}

