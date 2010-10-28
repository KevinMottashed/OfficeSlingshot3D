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

/**
 * The virtual environment is the 3d environment in which the game is played.
 * This class contains all the 3d objects that make up our game.
 * The environment is implemented using the chai3d library.
 * This class is responsible for all modifications to the environment.
 */
class VirtualEnvironment
{
public:
	/**
	 * Constructor
	 */
	VirtualEnvironment(void);

	/**
	 * Destructor
	 */
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
	 * Reduce the hit points on the local player
	 * @param dmg The amount of hit points to remove.
	 */
	void reduceLocalHp(unsigned int dmg);
	
	/**
	 * Reduce the hit points on the peer player
	 * @param dmg The amount of hit points to remove.
	 */
	void reducePeerHp(unsigned int dmg);


	/**
	 * Resets all the environment objects
	 */
	void resetAll();

	/**
	 * Check if the local player has no more HP
	 * @return True if local player has no more HP
	 */
	bool isLocalPlayerDead();

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

	/**
	 * Initialize the 3d environment.
	 * This initializes all the 3d objects.
	 */
	void initialize(void);
	
	/**
	 * Update the frame.
	 * This will update the frame to display the latest changes.
	 * This is where the physics are calculated and object positions are updated.
	 */
	void updateFrame();

	/**
	 * Check if an object is colliding with the local player.
	 * @return true If an object is hitting the local player.
	 */
	bool isColliding();

	/**
	 * Get the current ball's position
	 * @return The current ball's position
	 */
	cVector3d getCurrentBallPosition();

	/**
	 * Get the body part hit by the ball
	 * @return The HumanPart body part index
	 */
	int getAvatarHitBodyPart();

	/**
	 * Retrieve the chest hit box minimum value from the local avatar object
	 * @return The local avatar's chest hit box's minimum
	 */
	cVector3d getLocalAvatarChestMin();

	/**
	 * Retrieve the chest hit box maximum value from the local avatar object
	 * @return The local avatar's chest hit box's maximum
	 */
	cVector3d getLocalAvatarChestMax();

	/**
	 * Retrieve the right arm hit box minimum value from the local avatar object
	 * @return The local avatar's right arm hit box's minimum
	 */
	cVector3d getLocalAvatarRightArmMin();

	/**
	 * Retrieve the right arm hit box maximum value from the local avatar object
	 * @return The local avatar's right arm hit box's maximum
	 */
	cVector3d getLocalAvatarRightArmMax();

	/**
	 * Retrieve the left arm hit box minimum value from the local avatar object
	 * @return The local avatar's left arm hit box's minimum
	 */
	cVector3d getLocalAvatarLeftArmMin();

	/**
	 * Retrieve the left arm hit box maximum value from the local avatar object
	 * @return The local avatar's left arm hit box's maximum
	 */
	cVector3d getLocalAvatarLeftArmMax();

	/**
	 * Retrieve the head hit box minimum value from the local avatar object
	 * @return The local avatar's head hit box's minimum
	 */
	cVector3d getLocalAvatarHeadMin();

	/**
	 * Retrieve the head hit box maximum value from the local avatar object
	 * @return The local avatar's head hit box's maximum
	 */
	cVector3d getLocalAvatarHeadMax();

private:

	/**
	 * The virtual environments camera.
	 * This is the camera through which the player will see the world.
	 */
	cCamera* _camera;

	/**
	 * The 3d world.
	 * This is the virtual environments world, all 3d objects will be added to this world.
	 */
	cWorld* world;

	/**
	 * The light that illuminates the world.
	 * This light is attached to the camera.
	 */
	cLight* light;

	/**
	 * The local player's slingshot.
	 */
	VirtualSlingshot* lSlingshot;
	
	/**
	 * The peer player's slingshot.
	 */
	VirtualSlingshot* rSlingshot;

	/**
	 * The local player's avatar
	 */
	VirtualAvatar* rAvatar;

	/** 
	 * The peer player's avatar.
	 */
	VirtualAvatar* lAvatar;

	/**
	 * The peer's hit point bar.
	 */
	HpBar* rHpBar;

	/**
	 * The local player's hit point bar.\
	 */
	HpBar* lHpBar;

	/**
	 * The background image.
	 */
	cBitmap* background;

	/**
	 * The ODE world.
	 * This is the world object used for physics.
	 * In contrast with the chai3d world which is used for displaying objects.
	 */
	cODEWorld* ODEWorld;

	/**
	 * The balls that can be fired by the local player.
	 */
	std::vector<VirtualBall*> localBalls;

	/**
	 * The balls that can be fired by the peer player.
	 */
	std::vector<VirtualBall*> peerBalls;

	/**
	 * The physical ground.
	 * This object is used for physics on the floor.
	 * Other 3d objects can hit the floor and bounce off of it.
	 */ 
	cODEGenericBody* ODEGround;

	/**
	 * The number of balls that have been fired by the peer.
	 */
	int rNumBalls;

	/**
	 * The number of balls that have been fired by the local player.
	 */
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
