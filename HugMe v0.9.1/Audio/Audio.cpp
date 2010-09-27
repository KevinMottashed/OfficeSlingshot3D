#include "Audio.h"

#include "stdafx.h"
#include "fmod_errors.h"

Audio::Audio()
{
	FMOD_RESULT result;

	// create and initialize the system
	result = FMOD::System_Create(&system);
	assert(result == FMOD_OK);
	result = system->init(100, FMOD_INIT_NORMAL, 0);
	assert(result == FMOD_OK);

	// create streams for all the sounds
	result = system->createStream("sounds/tink.mp3", FMOD_DEFAULT, 0, &hitSound);
	assert(result == FMOD_OK);
	result = system->createStream("sounds/round1.mp3", FMOD_DEFAULT, 0, &round1Sound);
	assert(result == FMOD_OK);
	result = system->createStream("sounds/doom3_theme.mp3", FMOD_LOOP_NORMAL | FMOD_2D | FMOD_HARDWARE, 0, &bgMusicSound);
	assert(result == FMOD_OK);
	result = system->createStream("sounds/victory.mp3", FMOD_DEFAULT, 0, &gameWonSound);
	assert(result == FMOD_OK);
	result = system->createStream("sounds/game_over.mp3", FMOD_DEFAULT, 0, &gameLostSound);
	assert(result == FMOD_OK);
}

Audio::~Audio()
{
	FMOD_RESULT result = system->release();
	assert(result == FMOD_OK);
}

void Audio::playBGMusic()
{
	FMOD_RESULT result = system->playSound(FMOD_CHANNEL_FREE, bgMusicSound, false, NULL);
	assert(result == FMOD_OK);
}

void Audio::playHit()
{
	FMOD_RESULT result = system->playSound(FMOD_CHANNEL_FREE, hitSound, false, NULL);
	assert(result == FMOD_OK);
}

void Audio::playGameStart()
{
	FMOD_RESULT result = system->playSound(FMOD_CHANNEL_FREE, round1Sound, false, NULL);
	assert(result == FMOD_OK);
}

void Audio::playGameOverWon()
{
	FMOD_RESULT result = system->playSound(FMOD_CHANNEL_FREE, gameWonSound, false, NULL);
	assert(result == FMOD_OK);
}

void Audio::playGameOverLost()
{
	FMOD_RESULT result = system->playSound(FMOD_CHANNEL_FREE, gameLostSound, false, NULL);
	assert(result == FMOD_OK);
}
