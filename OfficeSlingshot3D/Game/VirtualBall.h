#ifndef VIRTUAL_BALL_H
#define VIRTUAL_BALL_H

#include "stdafx.h"
#include "Projectile.h"
#include "chai3d.h"
#include "CODE.h"
#include "Projectile.h"
#include "WorldConstants.h"

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
	 * @param isLocal True if its a local ball
	 */
	VirtualBall(cWorld* world, cODEWorld* ODEWorld, bool isLocal);

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
	void fire(const Projectile& p);

	/**
	 * Get the ball's position.
	 * @return The position of the ball.
	 */
	cVector3d getBallPos() const;

	/**
	 * Get the ball's center.
	 * @return The center of the ball.
	 */
	cVector3d getBallCenter() const;

	/**
	 * Get the alreadyCollided flag
	 * @return The alreadyCollided flag
	 */
	bool getAlreadyCollided() const;

	/**
	 * Sets the alreadyCollided flag to true to make sure that
	 * the ball only collides once for each time it was fired
	 */
	void collided();

	/**
	 * Resets the ball by disabling it and resetting all flags
	 */
	void reset();

	/**
	 * Moves the ball within the ball boundaries
	 */
	void move(const cVector3d& newBallPos);

	/**
	 * Translates the initial and final positions of the ball to a force
	 * @return The force corresponding to the start and end positions
	 */
	cVector3d calculateForceVector();

private:
	/**
	 * The mesh that represents the ball.
	 */
	cMesh* ballMesh;

	/**
	 * True if the ball is local
	 */
	bool isLocal;

	/**
	 * Flag to know if the ball has already collided
	 */
	bool alreadyCollided;

	/**
	 * The ball's physical representation.
	 * This is used for collisions and other physics calculations.
	 */
	cODEGenericBody* odeBall;

	/**
	 * Variable capturing if its the first pullback message received
	 */
	bool firstPullBack;

	/**
	 * The offset of the starting position of the ball compared 
	 * to the starting position of the ball
	 */
	cVector3d startingOffset;
};

#endif
