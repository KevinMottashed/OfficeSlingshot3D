#include "PerspectiveMath.h"

void PerspectiveMath::invertPerspective(cVector3d& vector)
{
	vector.x = -vector.x;
	vector.y = -vector.y;
}

void PerspectiveMath::invertPerspective(Projectile& projectile)
{
	// invert position
	cVector3d adjustedPosition = projectile.position();
	invertPerspective(adjustedPosition);
	projectile.position(adjustedPosition);

	// invert force
	cVector3d adjustedForce = projectile.force();
	invertPerspective(adjustedForce);
	projectile.force(adjustedForce);
}