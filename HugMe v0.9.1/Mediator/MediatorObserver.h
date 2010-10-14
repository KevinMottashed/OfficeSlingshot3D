#ifndef MEDIATOR_OBSERVER_H
#define MEDIATOR_OBSERVER_H

#include "stdafx.h"
#include "MediatorUpdateContextEnum.h"

/**
 * @ingroup Mediator
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
	 * @param context The update's context.
	 * @param data The data associated with the update.
	 */
	virtual void update(MediatorUpdateContext_t context, const void* data) = 0;
};

#endif

