#ifndef NETWORK_OBSERVER_H
#define NETWORK_OBSERVER_H

#include "stdafx.h"

// each update has a context
// the context represents what has changed in the observed object
enum NetworkUpdateContext
{
	PEER_CONNECTED,
	PEER_DISCONNECTED,
	PEER_START_GAME,
	PEER_PAUSE_GAME,
	PEER_EXIT_GAME,
	NETWORK_ERROR,
	RECEIVED_USER_NAME,
	RECEIVED_CHAT_MESSAGE,
	RECEIVED_VIDEO,
	RECEIVED_SLINGSHOT_PULLBACK,
	RECEIVED_PROJECTILE,
	RECEIVED_PLAYER_POSITION,
	RECEIVED_GAME_OVER,
	RECEIVED_HEALTH_LOST,
};

class NetworkObserver
{
public:
	NetworkObserver();
	virtual ~NetworkObserver();

	// handles an update
	virtual void update(NetworkUpdateContext context, const void* data) = 0;
};

#endif

