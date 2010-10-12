#ifndef VIRTUAL_AVATAR_H
#define VIRTUAL_AVATAR_H

#include "chai3d.h"

/**
 * Represents an avatar in the 3d environment.
 */
class VirtualAvatar
{
public:
	/**
	 * Contructor.
	 * @param world The chai3d world to which the avatar belongs.
	 * @param startingPosition The avatar's position.
	 * @param true if the avatar is the local avatar.
	 */
	VirtualAvatar(cWorld* world, cVector3d startingPosition, bool isLocal);

	/** 
	 * Destructor
	 */
	~VirtualAvatar(void);

	/**
	 * Rotate the avatar.
	 * @param ang The angle to rotate the avatar to. This angle is absolute to the starting position, not relative.
	 */
	void rotate(double ang);

	/**
	 * Translate (move) the avatar.
	 * @param ang The angle to which the avatar was rotated.
	 */
	void translate(double ang);

	/**
	 * Update boundaries when the avatar is moved or rotated.
	 * @param ang The angle from which the avatar was moved.
	 * @param position The avatar's position.
	 */
	void updateBoundaries(double ang, cVector3d position);

	/**
	 * Returns true if the point is in the balls hit box.
	 */
	bool isInHitBox(cVector3d ballPos);
private:
	/**
	 * The mesh that displays the avatar.
	 */
	cMesh* avatarMesh;

	/**
	 * The minimum values of the bounding box.
	 */
	cVector3d boundaryMin;

	/**
	 * The maximum values of the bounding box.
	 */
	cVector3d boundaryMax;

	/**
	 * True if the avatar belongs to the local player.
	 */
	bool isLocal;

};

#endif
