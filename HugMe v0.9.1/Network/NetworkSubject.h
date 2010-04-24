#ifndef NETWORK_SUBJECT_H_
#define NETWORK_SUBJECT_H_

#include "stdafx.h"
#include "NetworkObserver.h"

class NetworkSubject
{
public:
	NetworkSubject();
	virtual ~NetworkSubject();

	void attach(NetworkObserver* observer);
	void detach(NetworkObserver* observer);

	void notify(NetworkUpdateContext context, const void* data = NULL);

private:
	std::vector<NetworkObserver*> m_observers;
};

#endif