#ifndef AUDIO_TEST_SUITE_H
#define AUDIO_TEST_SUITE_H

#include "stdafx.h"
#include "TestSuite.h"
#include "AudioProxy.h"

// This test suite tests everything audio related
class AudioTestSuite : public TestSuite
{
public:
	/**
	 * Constructor
	 * @param name The name of this test suite as it will appear in the test results.
	 */
	AudioTestSuite(const std::string& name = "");

	/**
	 * Destructor
	 */
	~AudioTestSuite();

	/**
	 * run all tests
	 */
	virtual void run();

private:

	/**
	 * Test the afterburn sound
	 */
	void testAfterburn();

	/**
	 * Test the big explosion sound
	 */
	void testBigExplosion();

	/**
	 * Test the explosion sound
	 */
	void testExplosion();

	/**
	 * Test the pew sound
	 */
	void testPew();

	/**
	 * Test the pow sound
	 */
	void testPow();

	/**
	 * Test the tink sound
	 */
	void testTink();

	/**
	 * Test the tuck sound
	 */
	void testTuck();

	/**
	 * Generic test case for testing an audio file.
	 * The file is loaded and then played.
	 * @param fileName The name of the file to load
	 */
	void testAudioFile(const std::string& fileName);

	/**
	 * Test playing the background music
	 */
	void testPlayBGMusic();

	/**
	 * Test playing the player hit sound
	 */
	void testPlayHit();

	/**
	 * Test playing the game start sound
	 */
	void testPlayGameStart();

	/**
	 * Test playing the game over won sound
	 */
	void testPlayGameOverWon();

	/**
	 * Test playing the game over lost sound
	 */
	void testPlayGameOverLost();

	/**
	 * Test mute/unmute.
	 */
	void testMute();

	/**
	 * Test volume adjust.
	 */
	void testVolumeAdjust();
};

#endif
