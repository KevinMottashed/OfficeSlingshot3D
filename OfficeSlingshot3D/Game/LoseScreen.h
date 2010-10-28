#ifndef LOSE_SCREEN_H
#define LOSE_SCREEN_H

#include "chai3d.h"

/**
 * The lose screen is a 3D environment that is displayed when the local player lost the game
 */
class LoseScreen
{
public:
	/**
	 * Constructor
	 */
	LoseScreen();

	/**
	 * Destructor
	 */
	virtual ~LoseScreen();

	/**
	 * Retrieve the camera for this title screen
	 * @return The camera that should be used to render this environment
	 */
	cCamera* camera();

private:
	/**
	 * The virtual world.
	 */
	cWorld* world;

	/**
	 * The world's camera.
	 */
	cCamera* _camera;

	/**
	 * The environment's light.
	 */
	cLight* light;

	/**
	 * The background bitmap image.
	 */
	cBitmap* bitmap;
};

#endif