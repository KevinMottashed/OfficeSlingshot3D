#include "TestFailedException.h"

using namespace std;

TestFailedException::TestFailedException(const string& error) :
	error(error)
{
}

TestFailedException::~TestFailedException()
{
}

const char* TestFailedException::what() const throw()
{
	return error.c_str();
}

