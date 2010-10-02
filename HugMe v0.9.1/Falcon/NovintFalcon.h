#ifndef NOVINT_FALCON__H
#define NOVINT_FALCON__H

#include "stdafx.h"
#include "Falcon.h"

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

	// Creates a random 3dVector
	static DWORD getPositionFromFalcon(NovintFalcon* p_Falcon);

	/**
	 * Returns the box to which the slingshot is bound.
	 * @return the box to which the slingshot is bound
	 */
	virtual cCollisionAABBBox boundingBox() const;

private:
	NovintFalcon(const NovintFalcon& novintFalcon); // intentionally not implemented
	NovintFalcon& operator=(const NovintFalcon& novintFalcon); // intentionally not implemented

	bool falcon_enabled;
};

#endif

