#ifndef USER_INTERFACE_SUBJECT_H
#define USER_INTERFACE_SUBJECT_H

#include "stdafx.h"
#include "UserInterfaceObserver.h"

class UserInterfaceSubject
{
public:
	UserInterfaceSubject();
	virtual ~UserInterfaceSubject();

	void attach(UserInterfaceObserver* observer);
	void detach(UserInterfaceObserver* observer);

	void notify(UserInterfaceUpdateContext context, const void* data = NULL);

private:
	std::vector<UserInterfaceObserver*> m_observers;
};

#endif