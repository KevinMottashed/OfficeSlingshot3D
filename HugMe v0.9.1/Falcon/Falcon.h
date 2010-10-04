#ifndef FALCON_H
#define FALCON_H

#include "stdafx.h"
#include "FalconSubject.h"
#include "chai3d.h"

/**
 * Abstract class that represents a falcon.
 * The falcon will report events such as slingshot movement and slingshot firing.
 */
class Falcon : public FalconSubject
{
public:
	/**
	 * Constructor
	 */
	Falcon();

	/**
	 * Destructor.
	 */
	virtual ~Falcon();

	/**
	 * Start polling the falcon for information
	 */
	virtual void startPolling() = 0;

	/**
	 * Stop polling the falcon for information
	 */
	virtual void stopPolling() = 0;

	/**
	 * Returns the box to which the slingshot is bound.
	 * @return the box to which the slingshot is bound
	 */
	virtual cCollisionAABBBox boundingBox() const = 0;
};

#endif
