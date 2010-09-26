#ifndef USER_INTERFACE_OBSERVER_H
#define USER_INTERFACE_OBSERVER_H

#include "stdafx.h"

// each update has a context
// the context represents what has changed in the observed object
enum UserInterfaceUpdateContext
{
	LISTEN,
	CONNECT,
	DISCONNECT,
	USER_NAME,
	START_GAME,
	EXIT_GAME,
	PAUSE_GAME,
	EXIT_APPLICATION,
	CHAT_MESSAGE,
	ARM_BAND_PORT,
	JACKET_PORT,
	PREFERENCES,
	NEW_BALL
};

class UserInterfaceObserver
{
public:
	UserInterfaceObserver();
	virtual ~UserInterfaceObserver();

	// handles an update
	virtual void update(UserInterfaceUpdateContext context, const void* data) = 0;
};

#endif

