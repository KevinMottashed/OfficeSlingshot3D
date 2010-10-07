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

void PerspectiveMath::convertOrientationXYZtoYZX(cVector3d& vector) {
	double temp = vector.x;
	vector.x = vector.y;
	vector.y = vector.z;
	vector.z = temp;
}

void PerspectiveMath::convertBoxToBox(cVector3d& vector, const cCollisionAABBBox& boxOrigin, const cCollisionAABBBox& boxTarget) {
	vector.x = vector.x / (boxOrigin.getUpperX() - boxOrigin.getLowerX());
	vector.y = vector.y / (boxOrigin.getUpperY() - boxOrigin.getLowerY());
	vector.z = vector.z / (boxOrigin.getUpperZ() - boxOrigin.getLowerZ());

	vector.x = vector.x * (boxTarget.getUpperX() - boxTarget.getLowerX()) + boxTarget.getLowerX();
	vector.y = vector.y * (boxTarget.getUpperY() - boxTarget.getLowerY()) + boxTarget.getLowerY();
	vector.z = vector.z * (boxTarget.getUpperZ() - boxTarget.getLowerZ()) + boxTarget.getLowerZ();
}