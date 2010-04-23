#ifndef CONSOLE_LOGGER_H_
#define CONSOLE_LOGGER_H_

#include "Logger.h"
#include "ConsoleStream.h"

// concrete logger class
// this logger outputs the log in a human readable format to the visual studio output window
class ConsoleLogger : public Logger
{
public:
	ConsoleLogger();
	virtual ~ConsoleLogger();

protected:
	// logging functions
	virtual void logPeerConnected();
	virtual void logPeerDisconnected();
	virtual void logNetworkError(rc_network error);

private:
	ConsoleLogger(const ConsoleLogger& c); // intentionally not implemented
	ConsoleLogger& operator=(const ConsoleLogger& c); // intentionally not implemented
};

#endif