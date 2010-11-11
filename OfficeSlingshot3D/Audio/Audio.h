#ifndef AUDIO_H
#define AUDIO_H

#include "stdafx.h"
#include "FMOD.hpp"

#include "Sound.h"

/**
 * @ingroup Audio
 * Manages everything audio related.
 */
class Audio
{
public:
	/**
	 * Constructor
	 * This will preload all the audio files.
	 */
	Audio();

	/**
	 * Destructor
	 * Releases all resources related to the audio files.
	 */
	virtual ~Audio();

	/**
	 * play the background music (loops forever)
	 */
	void playBGMusic();

	/**
	 * stop the background music
	 */
	void stopBGMusic();

	/**
	 * play hit sound (when a player gets hit)
	 */
	void playHit();

	/**
	 * play headshot sound (when a player gets hit in the head)
	 */
	void playHeadshot();

	/**
	 * play slingshot fired sound
	 */
	void playSlingshotFired();

	/**
	 * play game start sound
	 */
	void playGameStart();

	/**
	 * play game over sound (when the player has won)
	 */
	void playGameOverWon();

	/**
	 * play game over sound (when the player has lost)
	 */
	void playGameOverLost();

	/**
	 * Mute all audio
	 */
	void mute();

	/**
	 * Unmute all audio
	 */
	void unmute();

	/**
	 * Set the volume level.
	 * @param level 0 for minimum, 1 for maximum
	 */
	void volume(float level);

private:
	
	/**
	 * Copy constructor
	 * Not-implemented to protect from use
	 */
	Audio(const Audio& other);

	/**
	 * Assignment operator
	 * Not-implemented to protect from use
	 */
	Audio& operator=(const Audio& other);

	/**
	 * The audio system.
	 * This variable manages all the audio.
	 */
	FMOD::System* system;

	std::auto_ptr<Sound> hitSound; /**< The sound when a player gets hit. */
	std::auto_ptr<Sound> headshotSound; /**< The sound when a player gets hit in the head. */
	std::auto_ptr<Sound> slingshotFiredSound; /**< The sound when a slingshot is fired. */
	std::auto_ptr<Sound> round1Sound; /**< The announcer when the game starts "round 1 .. fight!" */
	std::auto_ptr<Sound> bgMusicSound; /**< The background music, currently doom3 theme */
	std::auto_ptr<Sound> gameWonSound; /**< The announcer for when the game is won */
	std::auto_ptr<Sound> gameLostSound; /**< The announcer for when the game is lost */
};

#endif
