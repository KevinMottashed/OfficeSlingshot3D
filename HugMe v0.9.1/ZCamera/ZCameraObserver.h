#ifndef ZCAMERA_OBSERVER_H
#define ZCAMERA_OBSERVER_H

#include "stdafx.h"

// each update has a context
// the context represents what has changed in the observed object
enum ZCameraUpdateContext
{
	VIDEO,
	AVATAR_POSITION
};

class ZCameraObserver
{
public:
	ZCameraObserver();
	virtual ~ZCameraObserver();

	// handles an update
	virtual void update(ZCameraUpdateContext context, const void* data) = 0;
};

#endif

