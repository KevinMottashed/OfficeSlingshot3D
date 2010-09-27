#ifndef AUDIO_H
#define AUDIO_H

#include "FMOD.hpp"

/**
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
	 * play hit sound (when a player gets hit)
	 */
	void playHit();

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

	/**
	 * The sound when a player gets hit.
	 */
	FMOD::Sound* hitSound;

	/**
	 * The announcer when the game starts "round 1 .. fight!"
	 */
	FMOD::Sound* round1Sound;

	/**
	 * The background music, currently doom3 theme
	 */
	FMOD::Sound* bgMusicSound;

	/**
	 * The announcer for when the game is won
	 */
	FMOD::Sound* gameWonSound;

	/**
	 * The announcer for when the game is lost
	 */
	FMOD::Sound* gameLostSound;
};

#endif
