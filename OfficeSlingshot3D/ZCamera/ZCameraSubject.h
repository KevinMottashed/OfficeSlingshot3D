#ifndef ZCAMERA_SUBJECT_H
#define ZCAMERA_SUBJECT_H

#include "stdafx.h"
#include "ZCameraObserver.h"

/**
 * Abstract class in the observer pattern for the zcamera.
 * This class is used to report zcamera related updates to the observers.
 */
class ZCameraSubject
{
public:
	ZCameraSubject();
	virtual ~ZCameraSubject();

	/**
	 * Attach a new observer to this subject.
	 * @param observer The observer to attach to this subject. No duplicated are allowed.
	 */
	void attach(ZCameraObserver* observer);

	/**
	 * Detach an observer from this subject.
	 * @param observer The observer to detach from this subject.
	 */
	void detach(ZCameraObserver* observer);

	/**
	 * Notifies all observers that a new update is available.
	 * @param context The context of the update
	 * @param data The data associated with the update
	 */
	void notify(ZCameraUpdateContext context, const void* data = NULL);

private:
	/**
	 * The list of observers currently observing this subject.
	 */
	std::vector<ZCameraObserver*> m_observers;
};

#endif

