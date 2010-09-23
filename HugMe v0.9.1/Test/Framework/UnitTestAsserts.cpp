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