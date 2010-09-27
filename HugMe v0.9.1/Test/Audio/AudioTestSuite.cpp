#include "AudioTestSuite.h"

#include "FMOD.hpp"
#include "fmod_errors.h"

using namespace std;

AudioTestSuite::AudioTestSuite(const std::string& name) :
	TestSuite(name)
{

}

AudioTestSuite::~AudioTestSuite()
{

}

void AudioTestSuite::run()
{
	UNIT_TEST(testAfterburn);
	UNIT_TEST(testBigExplosion);
	UNIT_TEST(testExplosion);
	UNIT_TEST(testPew);
	UNIT_TEST(testPow);
	UNIT_TEST(testTink);
	UNIT_TEST(testTuck);
	UNIT_TEST(testPlayBGMusic);
	UNIT_TEST(testPlayHit);
	UNIT_TEST(testPlayGameStart);
	UNIT_TEST(testPlayGameOverWon);
	UNIT_TEST(testPlayGameOverLost);
}

void AudioTestSuite::testAfterburn()
{
	testAudioFile("sounds/afterburn.mp3");
}

void AudioTestSuite::testBigExplosion()
{
	testAudioFile("sounds/big_explosion.mp3");
}

void AudioTestSuite::testExplosion()
{
	testAudioFile("sounds/explosion.mp3");
}

void AudioTestSuite::testPew()
{
	testAudioFile("sounds/pew.mp3");
}

void AudioTestSuite::testPow()
{
	testAudioFile("sounds/pow.mp3");
}

void AudioTestSuite::testTink()
{
	testAudioFile("sounds/tink.mp3");
}

void AudioTestSuite::testTuck()
{
	testAudioFile("sounds/tuck.mp3");
}

void AudioTestSuite::testAudioFile(const string& fileName)
{
	FMOD_RESULT result;
	FMOD::System *system;

	result = FMOD::System_Create(&system);
	assert_equals(result, FMOD_OK, "Failed to create system object");

	result = system->init(100, FMOD_INIT_NORMAL, 0);
	assert_equals(result, FMOD_OK, "Failed to initialize FMOD");

	FMOD::Sound *sound;
	result = system->createStream(fileName.c_str(), FMOD_DEFAULT, 0, &sound);
	assert_equals(result, FMOD_OK, "Failed to create stream from file");

	FMOD::Channel *channel;
	result = system->playSound(FMOD_CHANNEL_FREE, sound, false, &channel);
	assert_equals(result, FMOD_OK, "Failed to play sound");

	// Sleep for the length of the sound
	unsigned int length;
	result = sound->getLength(&length, FMOD_TIMEUNIT_MS);
	assert_equals(result, FMOD_OK, "Should be able to retrieve sound length");

	Sleep(length);

	result = system->release();
	assert_equals(result, FMOD_OK, "Should be able to release all resources");
}

void AudioTestSuite::testPlayBGMusic()
{
	Audio audio;
	audio.playBGMusic();

	// only play the start of the song to save time
	Sleep(10000);
}

void AudioTestSuite::testPlayHit()
{
	Audio audio;
	audio.playHit();

	// 5 seconds should be enough to hear the whole sound
	Sleep(5000);
}

void AudioTestSuite::testPlayGameStart()
{
	Audio audio;
	audio.playGameStart();

	// 5 seconds should be enough to hear the whole sound
	Sleep(5000);
}

void AudioTestSuite::testPlayGameOverWon()
{
	Audio audio;
	audio.playGameOverWon();

	// 5 seconds should be enough to hear the whole sound
	Sleep(5000);
}

void AudioTestSuite::testPlayGameOverLost()
{
	Audio audio;
	audio.playGameOverLost();

	// 5 seconds should be enough to hear the whole sound
	Sleep(5000);
}
