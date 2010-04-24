#include "Logger.h"

Logger::Logger()
{
}

Logger::~Logger()
{
}

void Logger::update(NetworkUpdateContext context, const void *data)
{
	switch (context)
	{
		case PEER_CONNECTED:
		{
			logPeerConnected();
			break;
		}
		case PEER_DISCONNECTED:
		{
			logPeerDisconnected();
			break;
		}
		case NETWORK_ERROR:
		{
			assert(data != NULL);
			rc_network* error = (rc_network*) data;
			logNetworkError(*error);
			break;
		}
		default:
		{
			break;
		}
	}
	return;
}