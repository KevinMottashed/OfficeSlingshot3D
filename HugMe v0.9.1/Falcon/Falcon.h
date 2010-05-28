#ifndef FALCON_H
#define FALCON_H

#include "FalconSubject.h"

// Abstract class that represents a falcon
class Falcon : public FalconSubject
{
public:
	Falcon();
	virtual ~Falcon();

	// start polling the falcon for information (position, buttons, ...)
	virtual void startPolling() = 0;

	// stop polling the falcon for information (position, buttons, ...)
	virtual void stopPolling() = 0;
};

#endif
