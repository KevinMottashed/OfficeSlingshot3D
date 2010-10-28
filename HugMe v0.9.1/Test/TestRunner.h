#ifndef TEST_RUNNER_H
#define TEST_RUNNER_H

#include "stdafx.h"

#include "TestSuite.h"

/**
 * This class is used to start all the unit tests
 */
class TestRunner
{
public:
	/**
	 * Constructor
	 */
	TestRunner();

	/**
	 * Destructor
	 */
	~TestRunner();

	/**
	 * Run all tests suites
	 */
	void run();

	/**
	 * Print the test results to a stream
	 * @param os The stream to which the results will be saved
	 */
	void toStream(std::ostream& os) const;

private:
	/**
	 * A collection of all the test suites
	 */
	std::vector<TestSuite*> suites;
};

/**
 * Output Operator. Prints the test results to a stream
 * @param os The stream to which the results will be saved
 * @param runner The test runner where the results will be retrieved from
 * @return os with the test results
 */
std::ostream& operator<<(std::ostream& os, const TestRunner& runner);

#endif
