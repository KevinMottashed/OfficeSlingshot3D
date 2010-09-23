#ifndef TEST_SUITE_H
#define TEST_SUITE_H

#include "stdafx.h"

#include "UnitTestMacros.h"
#include "UnitTestAsserts.h"
#include "TestFailedException.h"

/**
 * This class is used to represent a collection of tests,
 * the tests can be run and the results can be outputed to a stream.
 */
class TestSuite
{
public:
	TestSuite(const std::string& name = "");
	~TestSuite();

	// run all tests
	virtual void run() = 0;

	// setup for each unit test, this is run before each unit test
	virtual void setup();

	// tear down each unit test, will be run after each test, 
	// usefull for deallocating resources allocated in setup
	virtual void tearDown();

	// print to stream
	void toStream(std::ostream& os) const;

protected:
	// the name of the test suite
	std::string name;

	// test results
	unsigned int totalTests;
	unsigned int failedTests;
	std::vector<std::string> results;
};

std::ostream& operator<<(std::ostream& os, const TestSuite& suite);

#endif
