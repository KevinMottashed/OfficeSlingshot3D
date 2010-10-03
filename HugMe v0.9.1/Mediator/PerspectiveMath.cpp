#include "PerspectiveMath.h"

void PerspectiveMath::invert2DCoordinate(cVector3d& vector)
{
	vector.x = -vector.x;
}

void PerspectiveMath::invert3DCoordinate(cVector3d& vector)
{
	vector.x = -vector.x;
	vector.y = -vector.y;
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