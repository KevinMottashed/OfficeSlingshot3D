#include "ConsoleStream.h"

std::streamsize ConsoleSink::write(const char* s, std::streamsize n)
{
	// the string is not guaranteed to be null terminated
	// and the OutputDebugString call is expecting a null terminated string
	boost::scoped_array<char> buffer(new char[n+1]);
	memcpy(buffer.get(), s, n);
	buffer[n] = '\0';

	// write the string to the console
	OutputDebugString(buffer.get());
	return n;
}

ConsoleStream::ConsoleStream()
{
	this->open(sink);
}

ConsoleStream::~ConsoleStream()
{
}

