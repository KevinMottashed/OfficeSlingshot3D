#ifndef COMMON_TEST_SUITE_H
#define COMMON_TEST_SUITE_H

#include "stdafx.h"
#include "TestSuite.h"

// This test suite tests everything audio related
class CommonTestSuite : public TestSuite
{
public:
	/**
	 * Constructor.
	 * @param name The name of this test suite as it will appear in the test results.
	 */
	CommonTestSuite(const std::string& name = "");

	/**
	 * Destructor.
	 */
	~CommonTestSuite();

	/**
	 * Run all tests.
	 */
	virtual void run();

private:

	/**
	 * Test a good preferences file.
	 */
	void testGoodPreferences();

	/**
	 * Test a malformed preferences file.
	 */
	void testMalformedPreferences();

	/**
	 * Test a preferences file with a missing attribute.
	 */
	void testMissingPreferences();
};

#endif
