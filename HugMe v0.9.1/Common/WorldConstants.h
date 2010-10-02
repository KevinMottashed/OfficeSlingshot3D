#ifndef WORLD_CONSTANTS_H
#define WORLD_CONSTANTS_H

#include "chai3d.h"

/**
 * Contains a bunch of constants relating to the virtual world.
 * Mainly coordinates for starting positions and bounding boxes for objects
 */
namespace World
{
	/**
	 * Error margin for floating point comparisons
	 * We need this to make sure that an object on the edge of a box is considered within the box
	 * For example, if a coordinate has x = 5.0 and a box has min_x = 3.0 and max_x = 5.0 then the coordinate
	 * Is not considered inside of the box so we need "min_x = 3.0 - error_margin" and "max_x = 5.0 + error_margin"
	 */
	static const float error_margin = 0.001f;

	/**
	 * Our slingshot's starting position
	 */
	static const cVector3d local_slingshot_starting_position = cVector3d(5.0f, 0.0f, -1.2f);
	
	/**
	 * The peers slingshot's starting position
	 */
	static const cVector3d peer_slingshot_starting_position = cVector3d(-5.0f, 0.0f, -1.2f);

	/**
	 * Local slingshot bounding box
	 */
	static const cCollisionAABBBox local_slingshot_bounding_box = cCollisionAABBBox(
		cVector3d(4.0f - error_margin, -1.0f - error_margin, -1.4f - error_margin), 
		cVector3d(5.0f + error_margin, 1.0f + error_margin, -1.0f + error_margin));

	/**
	 * Peer slingshot bounding box
	 */
	static const cCollisionAABBBox peer_slingshot_bounding_box = cCollisionAABBBox(
		cVector3d(-5.0f - error_margin, -1.0f - error_margin, -1.4f - error_margin), 
		cVector3d(-4.0f + error_margin, 1.0f + error_margin, -1.0f + error_margin));
}

#endif