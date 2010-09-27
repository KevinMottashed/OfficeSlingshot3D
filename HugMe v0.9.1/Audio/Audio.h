#ifndef AUDIO_H
#define AUDIO_H

#include "FMOD.hpp"

class Audio
{
public:
	Audio();
	virtual ~Audio();

	// play the background music (loops indefinatly)
	void playBGMusic();

	// play hit sound
	void playHit();

	// play game start
	void playGameStart();

	// play game over (won)
	void playGameOverWon();

	// play game over (lost)
	void playGameOverLost();

private:
	
	Audio(const Audio& other); // intentionally not implemented
	Audio& operator=(const Audio& other); // intentionally not implemented

	FMOD::System* system;
	FMOD::Sound* hitSound;
	FMOD::Sound* round1Sound;
	FMOD::Sound* bgMusicSound;
	FMOD::Sound* gameWonSound;
	FMOD::Sound* gameLostSound;
};

#endif