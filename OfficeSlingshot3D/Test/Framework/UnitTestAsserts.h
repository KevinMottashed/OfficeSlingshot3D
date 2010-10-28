#ifndef UNIT_TEST_ASSERTS
#define UNIT_TEST_ASSERTS

#include "stdafx.h"

#include "TestFailedException.h"

/**
 * Throws a test failed exception if a != b.
 * @param actual The value that was received
 * @param expected The value that was expected
 * @param message The message to display if the test fails
 */
template <typename S, typename T>
void assert_equals(const S& actual, const T& expected, const std::string& message = "");

/**
 * Throws a test failed exception if a != b.
 * This version is used for double and floats. 
 * A small margin of error is allowed due to the whole floating point comparison thing.
 * @param actual The value that was received
 * @param expected The value that was expected
 * @param message The message to display if the test fails
 * @param error_margin The error margin to use when comparing the 
 */
void assert_equals(const double& actual, const double& expected, const std::string& message = "", float error_margin = 0.001f);

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
	// some classes don't define the != operator so use the == operator instead
	if (!(actual == expected))
	{
		std::ostringstream os;
		os << "Actual=" << actual << " Expected=" << expected << " - " << message;
		throw TestFailedException(os.str());
	}
	return;
}

#endif
