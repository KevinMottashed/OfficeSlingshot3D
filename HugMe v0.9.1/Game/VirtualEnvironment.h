#ifndef VIRTUAL_ENVIRONMENT_H
#define VIRTUAL_ENVIRONMENT_H

#include "stdafx.h"
#include "boost.h"
#include "chai3d.h"
#include "CODE.h"
#include "Projectile.h"
#include "VirtualAvatar.h"
#include "VirtualSlingshot.h"
#include "VirtualBall.h"
#include "HpBar.h"

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

	void reduceLocalHp(int dmg);
	void reducePeerHp(int dmg);

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
	void updateFrame();
	bool isColliding();

private:
	cCamera* _camera;
	cWorld* world;
	cLight* light;
	VirtualSlingshot* lSlingshot;
	VirtualSlingshot* rSlingshot;
	VirtualAvatar* rAvatar;
	VirtualAvatar* lAvatar;
	HpBar* rHpBar;
	HpBar* lHpBar;
	cBitmap* background;

	cODEWorld* ODEWorld;
	std::vector<VirtualBall*> localBalls;
	std::vector<VirtualBall*> peerBalls;

	cODEGenericBody* ODEGround;

	int rNumBalls;
	int lNumBalls;

	/**
	 * The number of balls per player that can be in play at any given time
	 */
	static const unsigned int ball_limit;

	/**
	 * The force at which balls are fired.
	 * This should eventually be changed for a force that is
	 * calculated based on how far the sling is pulled.
	 */
	static const cVector3d firing_force;
};
#endif