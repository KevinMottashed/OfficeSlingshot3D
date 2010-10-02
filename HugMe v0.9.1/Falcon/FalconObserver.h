#ifndef FALCON_OBSERVER_H
#define FALCON_OBSERVER_H

#include "stdafx.h"

// each update has a context
// the context represents what has changed in the observed object
enum FalconUpdateContext
{
	SLINGSHOT_MOVED,
	SLINGSHOT_FIRED,
};

class FalconObserver
{
public:
	FalconObserver();
	virtual ~FalconObserver();

	// handles an update
	virtual void update(FalconUpdateContext context, const void* data) = 0;
};

#endif

