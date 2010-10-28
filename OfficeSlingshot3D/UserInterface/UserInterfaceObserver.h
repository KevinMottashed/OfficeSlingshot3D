#ifndef USER_INTERFACE_OBSERVER_H
#define USER_INTERFACE_OBSERVER_H

#include "stdafx.h"

/**
 * Enum.
 * Each update has a context.
 * The context represents what has changed in the observed object
 */
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
	MUTE_VOLUME,
};

/**
 * The observer class for the user interface.
 */
class UserInterfaceObserver
{
public:

	/**
	 * Constructor
	 */
	UserInterfaceObserver();

	/**
	 * Destructor
	 */
	virtual ~UserInterfaceObserver();

	/**
	 * Handles an update
	 * @param context The user interface context.
	 * @param data The data passed through the update. Can be NULL.
	 */
	// 
	virtual void update(UserInterfaceUpdateContext context, const void* data) = 0;
};

#endif

