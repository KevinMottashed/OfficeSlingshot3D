#ifndef MEDIATOR_OBSERVER_H
#define MEDIATOR_OBSERVER_H

#include "stdafx.h"
#include "MediatorUpdateContextEnum.h"

/**
 * Abstract class in the observer pattern.
 * The observer will receive notifications relating to user interaction with the system.
 */
class MediatorObserver
{
public:
	/**
	 * Constructor
	 */
	MediatorObserver();

	/**
	 * Destructor
	 */
	virtual ~MediatorObserver();

	/**
	 * Function that is called everytime the mediator has a notification to deliver.
	 * This function should be used to handle the notifications.
	 */
	virtual void update(MediatorUpdateContext_t context, const void* data) = 0;
};

#endif

