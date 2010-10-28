#ifndef FALCON_SUBJECT_H
#define FALCON_SUBJECT_H

#include "stdafx.h"
#include "FalconObserver.h"

/**
 * Abstract class in the observer pattern for the falcon.
 * This class is used to report falcon related updates to the observers.
 */
class FalconSubject
{
public:
	/**
	 * Constructor.
	 */
	FalconSubject();

	/**
	 * Destructor.
	 */
	virtual ~FalconSubject();

	/**
	 * Attach a new observer to this subject.
	 * @param observer The observer to attach to this subject. No duplicated are allowed.
	 */
	void attach(FalconObserver* observer);

	/**
	 * Detach an observer from this subject.
	 * @param observer The observer to detach from this subject.
	 */
	void detach(FalconObserver* observer);

	/**
	 * Notifies all observers that a new update is available.
	 * @param context The context of the update, ex: slingshot moved.
	 * @param data The data associated with the update, ex: the slingshot's new position.
	 */
	void notify(FalconUpdateContext context, const void* data = NULL);

private:
	/**
	 * The list of observers currently observing this subject.
	 */
	std::vector<FalconObserver*> m_observers;
};

#endif

