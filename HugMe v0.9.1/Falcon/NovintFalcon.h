#ifndef NOVINT_FALCON__H
#define NOVINT_FALCON__H

#include "Falcon.h"
#include "FalconProxy.h"
#include "chai3d.h"

// a concrete implementation of the falcon class
class NovintFalcon : public Falcon
{
public:
	NovintFalcon();
	virtual ~NovintFalcon();

	// start managing the falcon pen (polling it for information)
	// create a thread to poll it
	virtual void startPolling();

	// stop managing the falcon pen
	// stop the thread that's polling it
	virtual void stopPolling();
	/**
	 * Returns the box to which the slingshot is bound.
	 * @return the box to which the slingshot is bound
	 */
	virtual cCollisionAABBBox boundingBox() const;

	

private:
	NovintFalcon(const NovintFalcon& novintFalcon); // intentionally not implemented
	NovintFalcon& operator=(const NovintFalcon& novintFalcon); // intentionally not implemented

	bool reporting;

	/**
	 * The falcon's current position
	 */
	cVector3d position;
};

#endif

