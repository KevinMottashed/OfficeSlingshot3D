#ifndef NETWORK_TEST_SUITE_H
#define NETWORK_TEST_SUITE_H

#include "stdafx.h"
#include "TestSuite.h"

// Sample test suite
class SampleTestSuite : public TestSuite
{
public:
	SampleTestSuite(const std::string& name = "");
	~SampleTestSuite();

	// run all tests
	virtual void run();

private:
	void testAssertEquals();
	void testAssertEqualsFail();
	void testAssertTrue();
	void testAssertTrueFail();
	void testAssertFalse();
	void testAssertFalseFail();
};

#endif
