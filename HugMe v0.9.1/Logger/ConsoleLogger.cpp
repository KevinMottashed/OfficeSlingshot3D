#include "ConsoleLogger.h"

ConsoleLogger::ConsoleLogger()
{
}

ConsoleLogger::~ConsoleLogger()
{
}

void ConsoleLogger::logPeerConnected()
{
	console << "peer connected\n";
	return;
}

void ConsoleLogger::logPeerDisconnected()
{
	console << "peer disconnected\n";
	return;
}

void ConsoleLogger::logNetworkError(rc_network error)
{
	console << "network error " << lookup(error) << "\n";
	return;
}