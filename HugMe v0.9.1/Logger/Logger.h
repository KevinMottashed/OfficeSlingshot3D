#ifndef LOGGER_H_
#define LOGGER_H_

#include "NetworkProxy.h"

// abstract class used for logging
class Logger : public NetworkObserver
{
public:
	Logger();
	virtual ~Logger();

	// handle network updates, we will want to log these
	virtual void update(NetworkUpdateContext context, const void* data);	

protected:
	// log network events
	virtual void logPeerConnected() = 0;
	virtual void logPeerDisconnected() = 0;
	virtual void logNetworkError(rc_network error) = 0;
};

#endif