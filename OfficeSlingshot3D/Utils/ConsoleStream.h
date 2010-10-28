#ifndef CONSOLE_H
#define CONSOLE_H

#include "stdafx.h"
#include "boost.h"

/**
 * A boost iostream sink that writes to the debug console.
 */
class ConsoleSink : public boost::iostreams::sink
{
public:
	/**
	 * Write to the sink.
	 * @param s The c style string to write.
	 * @param n The number of characters to write.
	 * @return The number of characters that were written
	 */
	std::streamsize write(const char* s, std::streamsize n);
};

/** 
 * A utility for writing to the debug console.
 * The goal of this class is to wrap the OutputDebugStr function into a STL compliant ostream.
 *
 * Sample Usage:
 * console << "some message to display" << std::endl;
 */
class ConsoleStream : public boost::iostreams::stream<ConsoleSink>
{
public:
	/**
 	 * Constructor
	 */
	ConsoleStream();

	/**
	 * Destructor
	 */
	~ConsoleStream();
private:
	/**
	 * The output sink where all the output will go
	 */
	ConsoleSink sink;
};

/**
 * The console stream for this application.
 * This global variable is provided as a utility so that we don't need to 
 * initialize a ConsoleStream object each time we want to output.
 */
static ConsoleStream console;

#endif

