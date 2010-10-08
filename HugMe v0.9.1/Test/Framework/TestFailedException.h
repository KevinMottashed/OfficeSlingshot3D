#ifndef TEST_FAILED_EXCEPTION_H
#define TEST_FAILED_EXCEPTION_H

#include "stdafx.h"

/**
 * The exception that occurs when a unit test fails
 */
class TestFailedException : public std::exception
{
public:
	/**
	 * Constructor
	 */
	TestFailedException(const std::string& error);

	/**
	 * Destructor
	 */
	~TestFailedException();

	/**
	 * Get a description of this exception
	 * @return a string describing the exception
	 */
	virtual const char* what() const throw();

private:
	/**
	 * The error message for this exception
	 */
	std::string error;
};

#endif