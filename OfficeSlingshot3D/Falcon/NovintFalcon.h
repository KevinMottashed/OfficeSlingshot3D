#ifndef NOVINT_FALCON__H
#define NOVINT_FALCON__H

#include "Falcon.h"
#include "stdafx.h"
#include "boost.h"
#include "chai3d.h"
#include "UserPreferences.h"
#include "Configuration.h"

/**
 * The novint falcon. This class controls the slingshot via a novint falcon.
 */
class NovintFalcon : public Falcon
{
public:
	/**
	 * Constructor.
	 * @param configuration A reference to the configuration file
	 */
	NovintFalcon(boost::shared_ptr<Configuration> configuration);

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
	 * Poll the slingshot's position and firing state
	 */
	void poll();

	/**
	 * True when the falcon should notify observers that an event occured.
	 */ 
	bool reporting;

	/**
	 * The falcon's current position
	 */
	cVector3d position;

	bool firing; /**< True if the slingshot is currently in firing mode */

	bool forceEnabled; /**< True if the force feedback from the falcon device is used */

	/**
	 * The bounding box to which the falcon is restricted
	 */
	cCollisionAABBBox falconBox;

	/**
	 * Holds the thread which will poll the falcon
	 */
	std::auto_ptr<boost::thread> pollingThread;

	/**
	 * number of haptic devices detected
	 */
	int numHapticDevices;

	/**
	 * number of haptic devices supported
	 */
	static const int MAX_DEVICES = 4;

	/**
	 * A table containing pointers to all haptic devices detected on this computer
	 */
	cGenericHapticDevice* hapticDevices[MAX_DEVICES];

	/**
	 * The handler for the haptic devices
	 */
	cHapticDeviceHandler handler;
};

#endif

