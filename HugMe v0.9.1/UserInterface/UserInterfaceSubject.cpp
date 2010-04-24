#include "UserInterfaceSubject.h"

UserInterfaceSubject::UserInterfaceSubject() : m_observers()
{
}

UserInterfaceSubject::~UserInterfaceSubject()
{
}

void UserInterfaceSubject::attach(UserInterfaceObserver* observer)
{
	// no duplicates allowed
	assert(std::find(m_observers.begin(), m_observers.end(), observer) == m_observers.end());

	m_observers.push_back(observer);
	unsigned int size = m_observers.size();
	return;
}

void UserInterfaceSubject::detach(UserInterfaceObserver* observer)
{
	m_observers.erase(std::remove(m_observers.begin(), m_observers.end(), observer), m_observers.end());
	return;
}

void UserInterfaceSubject::notify(UserInterfaceUpdateContext context, const void* data)
{
	std::vector<UserInterfaceObserver*>::iterator it;
	unsigned int size = m_observers.size();
	for (it = m_observers.begin(); it != m_observers.end(); ++it)
	{
		(*it)->update(context, data);
	}
	return;
}