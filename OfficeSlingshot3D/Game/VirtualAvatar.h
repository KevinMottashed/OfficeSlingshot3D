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
	 * @param isLocal True if the avatar is the local avatar.
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
	 * Returns true if the point is in the balls hit box.
	 * @param ballPos The position of the ball
	 * @return True if the ball collided with the local avatar
	 */
	bool isInHitBox(cVector3d ballPos);

	/**
	 * Retrieves the body part hit by the ball
	 * @return The body part index hit by the ball
	 */
	int getHitBodyPart();

	/**
	 * Retrieve the chest hit box minimum value
	 * @return The chest hit box's minimum
	 */
	cVector3d getChestMin();

	/**
	 * Retrieve the chest hit box maximum value
	 * @return The chest hit box's maximum
	 */
	cVector3d getChestMax();

	/**
	 * Retrieve the right arm hit box minimum value
	 * @return The right arm hit box's minimum
	 */
	cVector3d getRightArmMin();

	/**
	 * Retrieve the right arm hit box maximum value
	 * @return The right arm hit box's maximum
	 */
	cVector3d getRightArmMax();

	/**
	 * Retrieve the left arm hit box minimum value
	 * @return The left arm hit box's minimum
	 */
	cVector3d getLeftArmMin();

	/**
	 * Retrieve the left arm hit box maximum value
	 * @return The left arm hit box's maximum
	 */
	cVector3d getLeftArmMax();

	/**
	 * Retrieve the head hit box minimum value
	 * @return The head hit box's minimum
	 */
	cVector3d getHeadMin();

	/**
	 * Retrieve the head hit box maximum value
	 * @return The head hit box's maximum
	 */
	cVector3d getHeadMax();

	/**
	 * Resets the avatar to its starting position
	 * @param startingPosition The avatar's starting position within the world
	 * @param isLocal True if the avatar is the local one
	 */
	void resetPosition(cVector3d startingPosition, bool isLocal);

private:
	/**
	 * The mesh that displays the avatar.
	 */
	cMesh* avatarMesh;

	/**
	 * The mesh for the chest hit box.
	 */
	cMesh* chestHitBox;

	/**
	 * The bottom-left corner of the chest bounding box 
	 */
	cVector3d chestMin;

	/**
	 * The top-right corner of the chest bounding box
	 */
	cVector3d chestMax;

	/**
	 * The mesh for the right arm hit box.
	 */
	cMesh* rArmHitBox;

	/**
	 * The bottom-left corner of the right arm bounding box 
	 */
	cVector3d rArmMin;

	/**
	 * The top-right corner of the right arm bounding box
	 */
	cVector3d rArmMax;

	/**
	 * The mesh for the left arm hit box.
	 */
	cMesh* lArmHitBox;

	/**
	 * The bottom-left corner of the left arm bounding box 
	 */
	cVector3d lArmMin;

	/**
	 * The top-right corner of the left arm bounding box
	 */
	cVector3d lArmMax;

	/**
	 * The mesh for the left arm hit box.
	 */
	cMesh* headHitBox;

	/**
	 * The bottom-left corner of the left arm bounding box 
	 */
	cVector3d headMin;

	/**
	 * The top-right corner of the left arm bounding box
	 */
	cVector3d headMax;
	/**
	 * True if the avatar belongs to the local player.
	 */
	bool isLocal;

	/**
	 * The body part index hit by the collision
	 */
	int hitPart;

	/**
	 * The initial position of the chest hit box's minimum value
	 */
	static const cVector3d iniChestMin;

	/**
	 * The initial position of the chest hit box's maximum value
	 */
	static const cVector3d iniChestMax;

	/**
	 * The initial position of the left arm hit box's minimum value
	 */
	static const cVector3d iniLeftArmMin;

	/**
	 * The initial position of the left arm hit box's maximum value
	 */
	static const cVector3d iniLeftArmMax;

	/**
	 * The initial position of the right arm hit box's minimum value
	 */
	static const cVector3d iniRightArmMin;

	/**
	 * The initial position of the right arm hit box's maximum value
	 */
	static const cVector3d iniRightArmMax;

	/**
	 * The initial position of the head hit box's minimum value
	 */
	static const cVector3d iniHeadMin;

	/**
	 * The initial position of the head hit box's maximum value
	 */
	static const cVector3d iniHeadMax;

	/**
	 * Creates the vertices associated to the boundary
	 * box for hit detection
	 * @param startingPosition The avatar's starting position
	 */
	void createBoundaries(cVector3d startingPosition);

	/**
	 * Creates the mesh object with the correct boundary points
	 * @param a_mesh The mesh object
	 * @param minVector Vector representing the minimum of the box
	 * @param maxVector Vector representing the maximum of the box
	 */
	void createMeshCube(cMesh* a_mesh, cVector3d minVector, cVector3d maxVector);

	/**
	 * Translate (move) the avatar.
	 * @param xTrans The x value to translate
	 * @param yTrans The y value to translate
	 */
	void translate(double xTrans, double yTrans);
};

#endif
