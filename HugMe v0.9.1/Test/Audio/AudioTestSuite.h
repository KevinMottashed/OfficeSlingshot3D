#ifndef AUDIO_TEST_SUITE_H
#define AUDIO_TEST_SUITE_H

#include "stdafx.h"
#include "TestSuite.h"

// This test suite tests the network connection establishment.
class AudioTestSuite : public TestSuite
{
public:
	AudioTestSuite(const std::string& name = "");
	~AudioTestSuite();

	// run all tests
	virtual void run();

private:

	// test each audio file that the application will use
	void testAfterburn();
	void testBigExplosion();
	void testExplosion();
	void testPew();
	void testPow();
	void testTink();
	void testTuck();

	// generic test case for testing an audio file
	// the file is loaded and then played
	void testAudioFile(const std::string& fileName);
};

#endif
