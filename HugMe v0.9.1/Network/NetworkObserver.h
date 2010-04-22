#ifndef NETWORK_OBSERVER_H_
#define NETWORK_OBSERVER_H_

#include "stdafx.h"

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
	RECEIVED_SLINGSHOT_POSITION,
	RECEIVED_PROJECTILE,
	RECEIVED_PULLBACK,
	RECEIVED_RELEASE,
	RECEIVED_PLAYER_POSITION
};

class NetworkObserver
{
public:
	NetworkObserver();
	virtual ~NetworkObserver();

	virtual void networkUpdate(NetworkUpdateContext context, void* data) = 0;
};

#endif