#ifndef MEDIATOR_OBSERVER_H
#define MEDIATOR_OBSERVER_H

#include "stdafx.h"

/** 
 * Each update has a context.
 * The context represents what has changed in the observed object
 */
enum MediatorUpdateContext
{
};

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
	virtual void update(MediatorUpdateContext context, const void* data) = 0;
};

#endif

