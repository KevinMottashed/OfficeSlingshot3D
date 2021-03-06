#ifndef MEDIATOR_SUBJECT_H
#define MEDIATOR_SUBJECT_H

#include "stdafx.h"
#include "MediatorObserver.h"

/**
 * @ingroup Mediator
 * @b public
 * This class represents the mediator in the observer pattern
 * Observers will receive notifications about various updates 
 * when the user interacts with the system.
 */
class MediatorSubject
{
public:
	/**
	 * Constructor.
	 */
	MediatorSubject();

	/**
	 * Destructor.
	 */
	virtual ~MediatorSubject();

	/**
	 * Attach an observer to this subject.
	 * @param[in] observer The observer to attach.
	 */
	void attach(MediatorObserver* observer);

	/**
	 * Detach an observer from this subject.
	 * @param[in] observer The observer to detach.
	 */
	void detach(MediatorObserver* observer);

	/**
	 * Notify all observers that an event happened.
	 * @param[in] context The context, describes what happened.
	 * @param[in] data The data associated with the update.
	 */
	void notify(MediatorUpdateContext_t context, const void* data = NULL);

private:
	/**
	 * List of observers observing this subject.
	 */
	std::vector<MediatorObserver*> observers;
};

#endif
