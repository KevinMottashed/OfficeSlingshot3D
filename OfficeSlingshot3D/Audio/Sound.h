#ifndef SOUND_H
#define SOUND_H

#include "stdafx.h"
#include "FMOD.hpp"

/**
 * @ingroup Audio
 * @b package
 * Abstraction for a Sound in the FMOD library.
 * This class makes it easier the play, stop and adjust volume levels.
 */
class Sound
{
public:
	/**
	 * Constructor.
	 * @param[in] system The FMOD system who will initialize the sound.
	 * @param[in] fileName the file from which to load the sound.
	 * @param[in] loop true if the sound should loop forever.
	 */
	Sound(FMOD::System* system, std::string fileName, bool loop);

	~Sound(); /**< Destructor. */

	/**
	 * Play this sound.
	 */
	void play();

	/**
	 * Stop playing this sound.
	 */
	void stop();

	/**
	 * Mute this sound.
	 */
	void mute();

	/**
	 * Unmute this sound.
	 */
	void unmute();

	/**
	 * Set the volume for this sound on a scale of 0 to 1 (max).
	 * @param[in] volume The volume level for this sound.
	 */
	void volume(float volume);

private:

	/**
	 * Helper function to set the channel to muted/unmuted
	 */
	void setMute();

	FMOD::System* system; /**< The FMOD system that will play the sounds */
	FMOD::Sound* sound; /**< The FMOD sound */
	FMOD::Channel* channel; /**< The FMOD channel on which the sound is playing */
	bool muted; /**< True if the sound is muted */
	float level; /**< The sound's volume level */
};

#endif