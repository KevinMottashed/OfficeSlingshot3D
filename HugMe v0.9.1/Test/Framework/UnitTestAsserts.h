#ifndef UNIT_TEST_ASSERTS
#define UNIT_TEST_ASSERTS

#include "stdafx.h"

#include "TestFailedException.h"

// throws a test failed exception if a != b
template <typename S, typename T>
void assert_equals(const S& actual, const T& expected, const std::string& message = "");

// throws a test failed exception if a is false
void assert_true(bool a, const std::string& message = "");

// throws a test failed exception if a is true
void assert_false(bool a, const std::string& message = "");

//--------------------------------------------
// TEMPLATE IMPLEMENTATIONS
//--------------------------------------------

template <typename S, typename T>
void assert_equals(const S& actual, const T& expected, const std::string& message)
{
	if (actual != expected)
	{
		std::ostringstream os;
		os << "Actual=" << actual << " Expected=" << expected << " - " << message;
		throw TestFailedException(os.str());
	}
	return;
}

#endif
