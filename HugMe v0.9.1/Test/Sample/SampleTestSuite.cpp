#include "SampleTestSuite.h"

SampleTestSuite::SampleTestSuite(const std::string& name) :
	TestSuite(name)
{
}

SampleTestSuite::~SampleTestSuite()
{
}

void SampleTestSuite::run()
{
	UNIT_TEST(testAssertEquals);
	UNIT_TEST(testAssertEqualsFail);
	UNIT_TEST(testAssertTrue);
	UNIT_TEST(testAssertTrueFail);
	UNIT_TEST(testAssertFalse);
	UNIT_TEST(testAssertFalseFail);
}

void SampleTestSuite::testAssertEquals()
{
	assert_equals(1, 1, "1 is not equal to 1");
}

void SampleTestSuite::testAssertEqualsFail()
{
	assert_equals(1, 2, "1 is not equal to 2");
}

void SampleTestSuite::testAssertTrue()
{
	assert_true(true, "True is not true");
}

void SampleTestSuite::testAssertTrueFail()
{
	assert_true(false, "False is not true");
}

void SampleTestSuite::testAssertFalse()
{
	assert_false(false, "False is not false");
}

void SampleTestSuite::testAssertFalseFail()
{
	assert_false(true, "True is not false");
}