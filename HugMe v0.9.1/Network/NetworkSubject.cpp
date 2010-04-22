#include "NetworkSubject.h"

NetworkSubject::NetworkSubject()
{
}

NetworkSubject::~NetworkSubject()
{
}

void NetworkSubject::attach(NetworkObserver* observer)
{
	// no duplicates allowed
	assert(std::find(m_observers.begin(), m_observers.end(), observer) == m_observers.end());

	m_observers.push_back(observer);
	return;
}

void NetworkSubject::detach(NetworkObserver* observer)
{
	m_observers.erase(std::remove(m_observers.begin(), m_observers.end(), observer), m_observers.end());
	return;
}

void NetworkSubject::networkNotify(NetworkUpdateContext context, void* data)
{
	std::vector<NetworkObserver*>::iterator it;
	for (it = m_observers.begin(); it != m_observers.end(); ++it)
	{
		(*it)->networkUpdate(context, data);
	}
}