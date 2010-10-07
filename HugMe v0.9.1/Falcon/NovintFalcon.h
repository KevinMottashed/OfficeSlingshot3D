#ifndef NOVINT_FALCON__H
#define NOVINT_FALCON__H

#include "Falcon.h"
#include "FalconProxy.h"
#include "chai3d.h"

/**
 * The novint falcon. This class controls the slingshot via a novint falcon.
 */
class NovintFalcon : public Falcon
{
public:
	/**
	 * Constructor.
	 */
	NovintFalcon();

	/**
	 * Destructor.
	 */
	virtual ~NovintFalcon();

	/**
	 * Start polling the falcon for information
	 */
	virtual void startPolling();

	/**
	 * Stop polling the falcon for information
	 */
	virtual void stopPolling();

	/**
	 * Returns the box to which the slingshot is bound.
	 * @return the box to which the slingshot is bound
	 */
	virtual cCollisionAABBBox boundingBox() const;

private:
	/**
	 * Copy constructor. Not implemented to protect from use.
	 * @param novintFalcon The object to copy.
	 */
	NovintFalcon(const NovintFalcon& novintFalcon);
 
	/**
	 * Assignment operator. Not implemented to protect from use.
	 * @param videoData The object to copy.
	 */
	NovintFalcon& operator=(const NovintFalcon& novintFalcon); 

	/**
	 * True when the falcon should notify observers that an event occured.
	 */ 
	bool reporting;

	/**
	 * The falcon's current position
	 */
	cVector3d position;

	cCollisionAABBBox falconBox;
};

#endif

