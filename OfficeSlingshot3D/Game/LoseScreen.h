#ifndef LOSE_SCREEN_H
#define LOSE_SCREEN_H

#include "stdafx.h"
#include "chai3d.h"

using namespace std;

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

	/**
	 * Changes between the mute and unmute icons
	 * @param soundOn True if the sound is currently on
	 */
	void changeMuteIcon(bool soundOn);

	/**
	 * Changes the volume control image
	 * @param vol The new volume
	 */
	void changeVolumeIcon(int vol);

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

	/**
	 * The mute control bitmap image.
	 */
	cBitmap* muteControl;

	/**
	 * The volume control bitmap image.
	 */
	cBitmap* volumeControl;
};

#endif