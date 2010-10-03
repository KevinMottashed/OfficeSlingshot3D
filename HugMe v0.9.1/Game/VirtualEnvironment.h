#ifndef VIRTUAL_ENVIRONMENT_H
#define VIRTUAL_ENVIRONMENT_H

#include "stdafx.h"
#include "boost.h"
#include "chai3d.h"
#include "CODE.h"
#include "Projectile.h"

class VirtualEnvironment
{
public:
	VirtualEnvironment(void);
	~VirtualEnvironment(void);

	/**
	 * Get the environments camera
	 * @return the camera
	 */
	cCamera* camera();

	/**
	 * Move the local slingshot to a new position
	 * @param position The new position
	 */
	void moveLocalSlingshot(cVector3d position);

	/**
	 * Move the peer slingshot to a new position
	 * @param position The new position
	 */
	void movePeerSlingshot(cVector3d position);
	
	/**
	 * Move the local avatar to a new position
	 * @param position The new position
	 */
	void moveLocalAvatar(cVector3d position);
	
	/**
	 * Move the peer avatar to a new position
	 * @param position The new position
	 */
	void movePeerAvatar(cVector3d position);

	/**
	 * Shoot a projectile from the local slingshot
	 * @return The projectile that was just fired
	 */
	Projectile fireLocalSlingshot();

	/**
	 * Shoot a projectile from the peer's slingshot
	 * @param p The projectile to be fired
	 */
	void firePeerSlingshot(Projectile p);

	void initialize(void);
	void createRectangle(cMesh* a_mesh, double width, double height, double depth);
	void updateFrame();

private:
	cCamera* _camera;
	cWorld* world;
	cLight* light;
	cMesh* rSlingshot;
	cMesh* lSlingshot;
	cMesh* rAvatar;
	cMesh* lAvatar;
	cMesh* ball;
	cMesh* ground;

	cODEWorld* ODEWorld;
	std::vector<cODEGenericBody*> localBalls;
	std::vector<cODEGenericBody*> peerBalls;

	cODEGenericBody* rODEAvatar;
	cODEGenericBody* lODEAvatar;
	cODEGenericBody* ODEGround;

	int rNumBalls;
	int lNumBalls;

	/**
	 * The number of balls per player that can be in play at any given time
	 */
	static const unsigned int ball_limit;

	/**
	 * This is the offset used when firing balls from the slingshot
	 * This offset makes the balls come out of the sling instead of the handle
	 */
	static const cVector3d slingshot_sling_offset;

	/**
	 * The force at which balls are fired.
	 * This should eventually be changed for a force that is
	 * calculated based on how far the sling is pulled.
	 */
	static const cVector3d firing_force;

};
#endif