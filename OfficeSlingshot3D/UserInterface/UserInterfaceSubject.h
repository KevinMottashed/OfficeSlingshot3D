#ifndef USER_INTERFACE_SUBJECT_H
#define USER_INTERFACE_SUBJECT_H

#include "stdafx.h"
#include "UserInterfaceObserver.h"

/**
 * A class needed for the subject/observer user pattern.
 */
class UserInterfaceSubject
{
public:
	/**
	 * Constructor
	 */
	UserInterfaceSubject();

	/**
	 * Destructor
	 */
	virtual ~UserInterfaceSubject();

	/**
	 * Attaches the subject to the observer.
	 * @param observer The observer.
	 */
	void attach(UserInterfaceObserver* observer);

	/**
	 * Detaches the subject to the observer.
	 * @param observer The observer.
	 */
	void detach(UserInterfaceObserver* observer);

	/**
	 * Notifies the observer of the new context.
	 * @param context The context to be updated.
	 * @param data The data being passed through with the update. Is NULL by default.
	 */
	void notify(UserInterfaceUpdateContext context, const void* data = NULL);

private:

	/**
	 * A vector of observers.
	 */
	std::vector<UserInterfaceObserver*> m_observers;
};

#endif

