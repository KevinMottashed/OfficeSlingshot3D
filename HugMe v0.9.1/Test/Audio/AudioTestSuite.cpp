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

	unsigned int length;
	result = sound->getLength(&length, FMOD_TIMEUNIT_MS);
	assert_equals(result, FMOD_OK, "Should be able to retrieve sound length");

	Sleep(length);

	result = system->release();
	assert_equals(result, FMOD_OK, "Should be able to release all resources");
}