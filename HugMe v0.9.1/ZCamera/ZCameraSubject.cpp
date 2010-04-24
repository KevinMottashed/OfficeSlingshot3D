#include "ZCameraSubject.h"

ZCameraSubject::ZCameraSubject() : m_observers()
{
}

ZCameraSubject::~ZCameraSubject()
{
}

void ZCameraSubject::attach(ZCameraObserver* observer)
{
	// no duplicates allowed
	assert(std::find(m_observers.begin(), m_observers.end(), observer) == m_observers.end());

	m_observers.push_back(observer);
	return;
}

void ZCameraSubject::detach(ZCameraObserver* observer)
{
	m_observers.erase(std::remove(m_observers.begin(), m_observers.end(), observer), m_observers.end());
	return;
}

void ZCameraSubject::notify(ZCameraUpdateContext context, const void* data)
{
	std::vector<ZCameraObserver*>::iterator it;
	for (it = m_observers.begin(); it != m_observers.end(); ++it)
	{
		(*it)->update(context, data);
	}
	return;
}