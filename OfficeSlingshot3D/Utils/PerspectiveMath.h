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

	/**
	 * Converts the orientation of a vector from XYZ to YZX.
	 * This effectively converts a perspective where x is depth, y is left-right and z is height to
	 * a perspective where x is left-right, y is height and z is depth.
	 * @param vector The vector to convert.
	 */
	static void convertOrientationNovintToGame(cVector3d& vector);

	static void convertOrientationGameToNovint(cVector3d& vector);


	/**
	 * Converts the orientation of a bounding box from XYZ to YZX
	 * See convertOrientationXYZtoYZX for more details
	 * @param box The bounding box to convert.
	 */
	static void convertBoxOrientationNovintToGame(cCollisionAABBBox& box);

	static void convertBoxOrientationGameToNovint(cCollisionAABBBox& box);

	/**
	 * Translates a point from one box to the other.
	 * The point appears at the same relative position in the target box.
	 * @param vector The point to translate.
	 * @param boxOrigin The box where the point originates from.
	 * @param boxTarget The box to which the point will be translated.
	 */
	static void convertBoxToBox(cVector3d& vector, const cCollisionAABBBox& boxOrigin, const cCollisionAABBBox& boxTarget);
};

#endif
