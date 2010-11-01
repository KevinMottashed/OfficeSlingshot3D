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

void PerspectiveMath::convertOrientationNovintToGame(cVector3d& vector) 
{
	//XYZ to YZX
	double temp = vector.x;
	vector.x = vector.y;
	vector.y = vector.z;
	vector.z = temp;
}

void PerspectiveMath::convertOrientationGameToNovint(cVector3d& vector) 
{
	//YZX to XYZ
	double temp = vector.y;
	vector.y = vector.x;
	vector.x = vector.z;
	vector.z = temp;
}

void PerspectiveMath::convertBoxOrientationNovintToGame(cCollisionAABBBox& box)
{
	PerspectiveMath::convertOrientationNovintToGame(box.m_min);
	PerspectiveMath::convertOrientationNovintToGame(box.m_max);
}

void PerspectiveMath::convertBoxOrientationGameToNovint(cCollisionAABBBox& box)
{
	PerspectiveMath::convertOrientationGameToNovint(box.m_min);
	PerspectiveMath::convertOrientationGameToNovint(box.m_max);
} 

void PerspectiveMath::convertBoxToBox(cVector3d& vector, const cCollisionAABBBox& boxOrigin, const cCollisionAABBBox& boxTarget) 
{
	// convert the vector to a unitary vector representing how far the coordinate is from the lower corner
	vector.x = (vector.x - boxOrigin.getLowerX()) / (boxOrigin.getUpperX() - boxOrigin.getLowerX());
	vector.y = (vector.y - boxOrigin.getLowerY()) / (boxOrigin.getUpperY() - boxOrigin.getLowerY());
	vector.z = (vector.z - boxOrigin.getLowerZ()) / (boxOrigin.getUpperZ() - boxOrigin.getLowerZ());

	// translate the vector to the relative position in the target box
	vector.x = vector.x * (boxTarget.getUpperX() - boxTarget.getLowerX()) + boxTarget.getLowerX();
	vector.y = vector.y * (boxTarget.getUpperY() - boxTarget.getLowerY()) + boxTarget.getLowerY();
	vector.z = vector.z * (boxTarget.getUpperZ() - boxTarget.getLowerZ()) + boxTarget.getLowerZ();
}