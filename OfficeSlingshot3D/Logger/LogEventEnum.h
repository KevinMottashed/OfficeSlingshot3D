#ifndef LOG_EVENT_H
#define LOG_EVENT_H

#include "stdafx.h"

// This enum represents all input events that the system can receive
// These include inputs from the network, the user interface, the falcon and the zcamera
namespace LogEvent
{
	// all the input events that can be logged
	enum LogEventEnum
	{
		// network events (0-99)
		START_NETWORK = 0,
		NETWORK_PEER_CONNECTED = START_NETWORK,
		NETWORK_PEER_DISCONNECTED,
		NETWORK_PEER_START_GAME,
		NETWORK_PEER_PAUSE_GAME,
		NETWORK_PEER_EXIT_GAME,
		NETWORK_ERROR_OCCURED,
		NETWORK_USER_NAME,
		NETWORK_CHAT_MESSAGE,
		NETWORK_VIDEO_DATA,
		NETWORK_SLINGSHOT_PULLBACK,
		NETWORK_PROJECTILE,
		NETWORK_PLAYER_POSITION,
		NETWORK_GAME_OVER,
		NETWORK_PLAYER_HIT,
		NETWORK_PHYSICS_SYNC,
		END_NETWORK = 100,

		// UI events (100-199)	
		START_UI = END_NETWORK,
		UI_CONNECT = START_UI,
		UI_LISTEN,
		UI_DISCONNECT,
		UI_PREFERENCES,
		UI_START_GAME,
		UI_PAUSE_GAME,
		UI_EXIT_GAME,
		UI_CLOSE_APPLICATION,
		UI_CHAT_MESSAGE,
		END_UI = 200,

		// Falcon events (200-299)
		START_FALCON = END_UI,
		FALCON_SLINGSHOT_PULLBACK = START_FALCON,
		FALCON_SLINGSHOT_FIRED,
		END_FALCON = 300,

		// ZCam events (300-399)
		START_ZCAM = END_FALCON,
		ZCAM_VIDEO_DATA = START_ZCAM,
		ZCAM_AVATAR_POSITION,
		END_ZCAM = 400
	};

	// convert the enum to a string
	std::string lookup(LogEventEnum logEvent);
}

typedef LogEvent::LogEventEnum LogEvent_t;

#endif
