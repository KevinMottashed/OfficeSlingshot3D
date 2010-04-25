#include "GameSubject.h"

GameSubject::GameSubject()
{
}

GameSubject::~GameSubject()
{
}

void GameSubject::attach(GameObserver* observer)
{
	// no duplicates allowed
	assert(std::find(m_observers.begin(), m_observers.end(), observer) == m_observers.end());

	m_observers.push_back(observer);
	return;
}

void GameSubject::detach(GameObserver* observer)
{
	m_observers.erase(std::remove(m_observers.begin(), m_observers.end(), observer), m_observers.end());
	return;
}

void GameSubject::notify(GameUpdateContext context, const void* data)
{
	std::vector<GameObserver*>::iterator it;
	for (it = m_observers.begin(); it != m_observers.end(); ++it)
	{
		(*it)->update(context, data);
	}
	return;
}

