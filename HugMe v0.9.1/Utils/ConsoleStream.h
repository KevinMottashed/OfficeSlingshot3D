#ifndef CONSOLE_H_
#define CONSOLE_H_

#include "stdafx.h"

// A utility class for writing to the console
// the goal of this class is to wrap the OutputDebugStr function
// this will allow using the output operator to print to the console
//
// Usage
// console << "some message to display\n";
//
// Limitations
// stream modifiers can't be applied
// for example std::endl can't be used (use "\n" instead)
class ConsoleStream
{
public:
	ConsoleStream();
	~ConsoleStream();
};

// output operator for the console stream
template <typename T>
ConsoleStream& operator<<(ConsoleStream& c, const T& t)
{
	// write t to a stream
	std::ostringstream os;
	os << t;

	// write the stream to the console
	OutputDebugString(os.str().c_str());

	return c;
}

// the console for this application
static ConsoleStream console;

#endif