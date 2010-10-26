#ifndef VIRTUAL_BALL_H
#define VIRTUAL_BALL_H

#include "stdafx.h"
#include "Projectile.h"
#include "chai3d.h"
#include "CODE.h"
#include "Projectile.h"

/**
 * Represents a ball in the 3d environment.
 */
class VirtualBall
{
public:
	/**
	 * Constructor.
	 * @param world The chai3d world to which the ball belongs.
	 * @param ODEWorld The physical world to which the ball belongs.
	 */
	VirtualBall(cWorld* world, cODEWorld* ODEWorld);

	/**
	 * Destructor
	 */
	~VirtualBall();

	/**
	 * Fire the ball.
	 * The ball will take the position of the passed projectile and
	 * it will be applied the force of the passed projectile.
	 * @param p The projectile which the ball will become.
	 */
	void fire(Projectile p);

	/**
	 * Get the ball's position.
	 * @return The position of the ball.
	 */
	cVector3d getMeshPos();

	/**
	 * Get the alreadyCollided flag
	 * @return The alreadyCollided flag
	 */
	bool getAlreadyCollided();

	/**
	 * Sets the alreadyCollided flag to true to make sure that
	 * the ball only collides once for each time it was fired
	 */
	void collided();

	/**
	 * Resets the ball by disabling it
	 */
	void reset();

private:
	/**
	 * The mesh that represents the ball.
	 */
	cMesh* ballMesh;

	/**
	 * Flag to know if the ball has already collided
	 */
	bool alreadyCollided;

	/**
	 * The ball's physical representation.
	 * This is used for collisions and other physics calculations.
	 */
	cODEGenericBody* odeBall;

};

#endif
