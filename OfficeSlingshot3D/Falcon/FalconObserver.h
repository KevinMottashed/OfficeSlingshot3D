#ifndef FALCON_OBSERVER_H
#define FALCON_OBSERVER_H

#include "stdafx.h"

/**
 * Update context enum for the falcon.
 * These represent the different events that a falcon can report
 */
enum FalconUpdateContext
{
	SLINGSHOT_FIRED, /**< The slingshot has been fired */
	SLINGSHOT_PULLBACK, /**< The slingshot is currently been pulled back */
};

/**
 * Abstract class in the observer patern for the falcon.
 * The falcon reports all events via the observer pattern
 */
class FalconObserver
{
public:
	/**
	 * Constructor.
	 */
	FalconObserver();

	/**
	 * Destructor.
	 */
	virtual ~FalconObserver();

	/**
	 * Abstract function to handle updates from the falcon.
	 * Every time there is an update the falcon subject will call this method.
	 * @param context The context of the update, ex: slingshot moved
	 * @param data The data associated with the update, ex: The slingshot's new position.
	 */
	virtual void update(FalconUpdateContext context, const void* data) = 0;
};

#endif

