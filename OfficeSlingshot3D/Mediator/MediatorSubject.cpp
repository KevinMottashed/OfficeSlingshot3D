#include "MediatorSubject.h"

MediatorSubject::MediatorSubject()
{
}

MediatorSubject::~MediatorSubject()
{
}

void MediatorSubject::attach(MediatorObserver* observer)
{
	// no duplicates allowed
	assert(std::find(observers.begin(), observers.end(), observer) == observers.end());

	observers.push_back(observer);
	return;
}

void MediatorSubject::detach(MediatorObserver* observer)
{
	observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
	return;
}

void MediatorSubject::notify(MediatorUpdateContext_t context, const void* data)
{
	std::vector<MediatorObserver*>::iterator it;
	for (it = observers.begin(); it != observers.end(); ++it)
	{
		(*it)->update(context, data);
	}
	return;
}

