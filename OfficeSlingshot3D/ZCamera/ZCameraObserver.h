#ifndef ZCAMERA_OBSERVER_H
#define ZCAMERA_OBSERVER_H

#include "stdafx.h"

/**
 * Update context enum for the zcamera.
 * These represent the different events that a zcamera can report
 */
enum ZCameraUpdateContext
{
	VIDEO,
	AVATAR_POSITION
};

/**
 * Abstract class in the observer patern for the zcamera.
 * The zcamera reports all events via the observer pattern
 */
class ZCameraObserver
{
public:
	/**
	 * Constructor
	 */
	ZCameraObserver();

	/**
	 * Destructor
	 */
	virtual ~ZCameraObserver();

	/**
	 * Abstract function to handle updates from the zcamera.
	 * Every time there is an update the zcamera subject will call this method.
	 * @param context The context of the update
	 * @param data The data associated with the update
	 */
	virtual void update(ZCameraUpdateContext context, const void* data) = 0;
};

#endif

