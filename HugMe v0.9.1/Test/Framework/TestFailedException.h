#ifndef TEST_FAILED_EXCEPTION_H
#define TEST_FAILED_EXCEPTION_H

#include "stdafx.h"

// The exception that occurs when a unit test fails
class TestFailedException : public std::exception
{
public:
	TestFailedException(const std::string& error);
	~TestFailedException();

	virtual const char* what() const throw();

private:
	std::string error;
};

#endif