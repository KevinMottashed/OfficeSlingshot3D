#include "FalconSubject.h"

FalconSubject::FalconSubject()
{
}

FalconSubject::~FalconSubject()
{
}

void FalconSubject::attach(FalconObserver* observer)
{
	// no duplicates allowed
	assert(std::find(m_observers.begin(), m_observers.end(), observer) == m_observers.end());

	m_observers.push_back(observer);
	return;
}

void FalconSubject::detach(FalconObserver* observer)
{
	m_observers.erase(std::remove(m_observers.begin(), m_observers.end(), observer), m_observers.end());
	return;
}

void FalconSubject::notify(FalconUpdateContext context, const void* data)
{
	std::vector<FalconObserver*>::iterator it;
	for (it = m_observers.begin(); it != m_observers.end(); ++it)
	{
		(*it)->update(context, data);
	}
	return;
}