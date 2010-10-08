#include "UnitTestAsserts.h"

void assert_true(bool a, const std::string& message)
{
	if (!a)
	{
		throw TestFailedException(message);
	}
	return;
}

void assert_false(bool a, const std::string& message)
{
	assert_true(!a, message);
	return;
}

void assert_equals(const double& actual, const double& expected, const std::string& message, float error_margin)
{
	bool fail = false;
	if (actual > expected)
	{
		fail = actual - expected > error_margin;
	}
	else
	{
		fail = expected - actual > error_margin;
	}

	if (fail)
	{
		std::ostringstream os;
		os << "Actual=" << actual << " Expected=" << expected << "Error Margin=" << error_margin << " - " << message;
		throw TestFailedException(os.str());
	}

	return;
}