#include "TestRunner.h"

#include "boost.h"

#include "ConnectionTestSuite.h"
#include "AudioTestSuite.h"
#include "PerspectiveMathTestSuite.h"

TestRunner::TestRunner()
{
	suites.push_back(new ConnectionTestSuite("Network Connection"));
	suites.push_back(new AudioTestSuite("Audio"));
	suites.push_back(new PerspectiveMathTestSuite("Perspective Math"));
}

TestRunner::~TestRunner()
{
	foreach(TestSuite* suite, suites)
	{
		delete suite;
	}
}

void TestRunner::run()
{
	foreach(TestSuite* suite, suites)
	{
		suite->run();
	}
}

void TestRunner::toStream(std::ostream& os) const
{
	foreach(TestSuite* suite, suites)
	{
		os << *suite << std::endl << std::endl;
	}
}

std::ostream& operator<<(std::ostream& os, const TestRunner& runner)
{
	runner.toStream(os);
	return os;
}
