#ifndef TEST_RUNNER_H
#define TEST_RUNNER_H

#include "stdafx.h"

#include "TestSuite.h"
#include "ConnectionTestSuite.h"
#include "AudioTestSuite.h"

/**
 * This class is used to start all the unit tests
 */
class TestRunner
{
public:
	TestRunner();
	~TestRunner();

	// run all tests
	void run();

	// print to a stream
	void toStream(std::ostream& os) const;

private:
	std::vector<TestSuite*> suites;
};

std::ostream& operator<<(std::ostream& os, const TestRunner& runner);

#endif
