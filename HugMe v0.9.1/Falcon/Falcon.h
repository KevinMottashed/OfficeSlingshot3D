#ifndef FALCON_H
#define FALCON_H

#include "stdafx.h"
#include "FalconSubject.h"
#include "chai3d.h"

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

	/**
	 * Returns the box to which the slingshot is bound.
	 * @return the box to which the slingshot is bound
	 */
	virtual cCollisionAABBBox boundingBox() const = 0;
};

#endif
