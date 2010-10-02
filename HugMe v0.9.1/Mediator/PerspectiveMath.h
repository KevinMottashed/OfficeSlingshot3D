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
	 * Invert the perspective on a vector
	 * @param vector The vector to invert
	 */
	static void invertPerspective(cVector3d& vector);

	/**
	 * Invert the perspective on a projectile
	 * @param projectile The vector to invert
	 */
	static void invertPerspective(Projectile& projectile);
};

#endif