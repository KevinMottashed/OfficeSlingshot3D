#ifndef NETWORK_SUBJECT_H
#define NETWORK_SUBJECT_H

#include "stdafx.h"
#include "NetworkObserver.h"

/**
 * @ingroup Network
 * @b public
 * This class represents the network in the observer pattern
 * Observers will receive updates whenever a message is received over the network.
 */
class NetworkSubject
{
public:
	NetworkSubject(); /**< Constructor. */
	virtual ~NetworkSubject(); /**< Destructor. */

	/**
	 * Attaches an observer to this subject.
	 * The observer will start receiving updates.
	 * @param[in] observer The observer to attach.
	 */
	void attach(NetworkObserver* observer);

	/**
	 * Detach an observer from this subject.
	 * The observer will no longer receive updates.
	 * @param[in] observer The observer to detach.
	 */
	void detach(NetworkObserver* observer);

	/**
	 * Notifies all observers of an update.
	 * @param[in] context The update's context.
	 * @param[in] data The data associated with the update.
	 */
	void notify(NetworkUpdateContext context, const void* data = NULL);

private:
	std::vector<NetworkObserver*> m_observers; /**< The list of observers. */
};

#endif

