#include "TestSuite.h"

#include "boost.h"

TestSuite::TestSuite(const std::string& name) :
	name(name),
	totalTests(0),
	failedTests(0)
{
}

TestSuite::~TestSuite()
{
}

void TestSuite::setup()
{
}

void TestSuite::tearDown()
{
}

void TestSuite::toStream(std::ostream& os) const
{
	os << name << " Test Suite" << std::endl;
	os << failedTests << " out of " << totalTests << " tests failed." << std::endl;
	foreach(string result, results)
	{
		os << result << std::endl;
	}
}

std::ostream& operator<<(std::ostream& os, const TestSuite& suite)
{
	suite.toStream(os);
	return os;
}
