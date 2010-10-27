#include "Audio.h"

#include "stdafx.h"
#include "fmod_errors.h"

Audio::Audio()
{
	// error code
	FMOD_RESULT result;

	// create and initialize the system
	result = FMOD::System_Create(&system);
	assert(result == FMOD_OK);
	result = system->init(100, FMOD_INIT_NORMAL, 0);
	assert(result == FMOD_OK);

	hitSound = std::auto_ptr<Sound>(new Sound(system, "sounds/tink.mp3", false));
	slingshotFiredSound = std::auto_ptr<Sound>(new Sound(system, "sounds/slingshot.mp3", false));
	round1Sound = std::auto_ptr<Sound>(new Sound(system, "sounds/round1.mp3", false));
	bgMusicSound = std::auto_ptr<Sound>(new Sound(system, "sounds/doom3_theme.mp3", true));
	gameWonSound = std::auto_ptr<Sound>(new Sound(system, "sounds/victory.mp3", false));
	gameLostSound = std::auto_ptr<Sound>(new Sound(system, "sounds/game_over.mp3", false));
}

Audio::~Audio()
{
	// we only need to release the system, as it will free all other resources
	FMOD_RESULT result = system->release();
	assert(result == FMOD_OK);
}

void Audio::playBGMusic()
{
	bgMusicSound->play();
}

void Audio::stopBGMusic()
{
	bgMusicSound->stop();
}

void Audio::playHit()
{
	hitSound->play();
}

void Audio::playSlingshotFired()
{
	slingshotFiredSound->play();
}

void Audio::playGameStart()
{
	round1Sound->play();
}

void Audio::playGameOverWon()
{
	gameWonSound->play();
}

void Audio::playGameOverLost()
{
	gameLostSound->play();
}

void Audio::mute()
{
	hitSound->mute();
	slingshotFiredSound->mute();
	round1Sound->mute();
	bgMusicSound->mute();
	gameWonSound->mute();
	gameLostSound->mute();
}

void Audio::unmute()
{
	hitSound->unmute();
	slingshotFiredSound->unmute();
	round1Sound->unmute();
	bgMusicSound->unmute();
	gameWonSound->unmute();
	gameLostSound->unmute();
}

void Audio::volume(float level)
{
	hitSound->volume(level);
	slingshotFiredSound->volume(level);
	round1Sound->volume(level);
	bgMusicSound->volume(level);
	gameWonSound->volume(level);
	gameLostSound->volume(level);
}


