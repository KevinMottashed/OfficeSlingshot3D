#ifndef WIN_SCREEN_H
#define WIN_SCREEN_H

#include "chai3d.h"

/**
 * The win screen is a 3D environment that is displayed when the local player won the game
 */
class WinScreen
{
public:
	/**
	 * Constructor
	 */
	WinScreen();

	/**
	 * Destructor
	 */
	virtual ~WinScreen();

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