#ifndef PERSPECTIVE_MATH_H
#define PERSPECTIVE_MATH_H

#include "stdafx.h"
#include "chai3d.h"
#include "Projectile.h"

/**
 * Utility class that the mediator will use to adjust the players perspectives so that they match
 */
class PerspectiveMath
{
public:
	/**
	 * Invert the perspective on a coordinate in a 2D space
	 * @param vector The coordinate to invert
	 */
	static void invert2DCoordinate(cVector3d& vector);

	/**
	 * Invert the perspective on a coordinate in a 3D space
	 * @param vector The coordinate to invert
	 */
	static void invert3DCoordinate(cVector3d& vector);

	/**
	 * Invert the perspective on a projectile traveling in a 3D space
	 * @param projectile The projectile to invert
	 */
	static void invert3DProjectile(Projectile& projectile);
};

#endif