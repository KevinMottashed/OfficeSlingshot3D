#ifndef CONSOLE_H
#define CONSOLE_H

#include "stdafx.h"
#include "boost.h"

// A utility for writing to the console
// the goal of this file is to wrap the OutputDebugStr function
// into a STL compliant string
//
// Usage
// console << "some message to display" << std::endl;


// A boost sink that writes to the debug console
class ConsoleSink : public boost::iostreams::sink
{
public:
	std::streamsize write(const char* s, std::streamsize n);
};

// a boost stream that writes to a console sink
class ConsoleStream : public boost::iostreams::stream<ConsoleSink>
{
public:
	ConsoleStream();
	~ConsoleStream();
private:
	ConsoleSink sink;
};

// the console stream for this application
static ConsoleStream console;

#endif

