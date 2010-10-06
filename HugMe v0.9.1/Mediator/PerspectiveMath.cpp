#include "PerspectiveMath.h"

void PerspectiveMath::invert2DCoordinate(cVector3d& vector)
{
	vector.z = -vector.z;
}

void PerspectiveMath::invert3DCoordinate(cVector3d& vector)
{
	vector.z = -vector.z;
	vector.x = -vector.x;
}

void PerspectiveMath::invert3DProjectile(Projectile& projectile)
{
	// invert position
	cVector3d adjustedPosition = projectile.position();
	invert3DCoordinate(adjustedPosition);
	projectile.position(adjustedPosition);

	// invert force
	cVector3d adjustedForce = projectile.force();
	invert3DCoordinate(adjustedForce);
	projectile.force(adjustedForce);
}