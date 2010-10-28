#include "Sound.h"

Sound::Sound(FMOD::System* system, std::string fileName, bool loop) :
	system(system),
	sound(NULL),
	channel(NULL),
	muted(false),
	level(1.0f)
{
	// error code
	FMOD_RESULT result;

	// create a stream for the sound
	// creating a stream will stream the file instead of preloading the whole thing
	// this reduces the time it takes for the application to load
	if (loop)
	{
		result = system->createStream(fileName.c_str(), FMOD_LOOP_NORMAL | FMOD_2D | FMOD_HARDWARE, 0, &sound);
	}	
	else
	{
		result = system->createStream(fileName.c_str(), FMOD_DEFAULT, 0, &sound);
	}
	assert(result == FMOD_OK);
}

Sound::~Sound()
{
}

void Sound::play()
{
	// start the sound as paused
	FMOD_RESULT result = system->playSound(FMOD_CHANNEL_FREE, sound, true, &channel);
	assert(result == FMOD_OK);

	// set the volume attributes and unpause
	result = channel->setMute(muted);
	assert(result == FMOD_OK);
	result = channel->setVolume(level);
	assert(result == FMOD_OK);
	result = channel->setPaused(false);
	assert(result == FMOD_OK);
	return;
}

void Sound::stop()
{
	if (channel == NULL)
	{
		// it's not playing
		return;
	}

	// stop the channel if it's playing
	bool playing;
	FMOD_RESULT result = channel->isPlaying(&playing);
	assert(result == FMOD_OK);
	if (playing)
	{
		result = channel->stop();
		assert(result == FMOD_OK);
		channel = NULL;
	}
	return;
}

void Sound::mute()
{
	muted = true;
	setMute();
}

void Sound::unmute()
{
	muted = false;
	setMute();
}

void Sound::volume(float volume)
{
	level = volume;

	if (channel == NULL)
	{
		// it's not playing
		return;
	}

	// set the channel's volume
	FMOD_RESULT result = channel->setVolume(level);
	assert(result == FMOD_OK);
	return;
}

void Sound::setMute()
{
	if (channel == NULL)
	{
		// it's not playing
		return;
	}

	// mute/unmute the channel
	FMOD_RESULT result = channel->setMute(muted);
	assert(result == FMOD_OK);
	return;
}