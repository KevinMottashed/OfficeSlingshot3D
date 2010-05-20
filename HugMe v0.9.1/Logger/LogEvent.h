#ifndef LOG_EVENT_H
#define LOG_EVENT_H

// all the events that can be logged
enum LogEvent
{
	NETWORK_PEER_CONNECTED,
	NETWORK_PEER_DISCONNECTED,
	NETWORK_PEER_START_GAME,
	NETWORK_PEER_PAUSE_GAME,
	NETWORK_PEER_EXIT_GAME,
	NETWORK_ERROR_OCCURED,
	NETWORK_USER_NAME,
	NETWORK_CHAT_MESSAGE,
	NETWORK_VIDEO_DATA,
	NETWORK_SLINGSHOT_POSITION,
	NETWORK_PROJECTILE,
	NETWORK_SLINGSHOT_PULLBACK,
	NETWORK_SLINGSHOT_RELEASE,
	NETWORK_PLAYER_POSITION,
	UI_CONNECT,
	UI_LISTEN,
	UI_DISCONNECT,
	UI_PREFERENCES,
	UI_START_GAME,
	UI_PAUSE_GAME,
	UI_EXIT_GAME,
	UI_CLOSE_APPLICATION,
	UI_CHAT_MESSAGE,
	FALCON_SLINGSHOT_POSITION,
	ZCAM_VIDEO_DATA
};

// convert the enum to a string
static std::string lookup(LogEvent logEvent)
{
	switch(logEvent)
	{
	case NETWORK_PEER_CONNECTED:
		return "Network - peer connected";
	case NETWORK_PEER_DISCONNECTED:
		return "Network - peer disconnected";
	case NETWORK_PEER_START_GAME:
		return "Network - peer start game";
	case NETWORK_PEER_PAUSE_GAME:
		return "Network - peer pause game";
	case NETWORK_PEER_EXIT_GAME:
		return "Network - peer exit game";
	case NETWORK_ERROR_OCCURED:
		return "Network - error occured";
	case NETWORK_USER_NAME:
		return "Network - user name";
	case NETWORK_CHAT_MESSAGE:
		return "Network - chat message";
	case NETWORK_VIDEO_DATA:
		return "Network - video data";
	case NETWORK_SLINGSHOT_POSITION:
		return "Network - slingshot position";
	case NETWORK_PROJECTILE:
		return "Network - projectile";
	case NETWORK_SLINGSHOT_PULLBACK:
		return "Network - slingshot pullback";
	case NETWORK_SLINGSHOT_RELEASE:
		return "Network - slingshot release";
	case NETWORK_PLAYER_POSITION:
		return "Network - player position";
	case UI_CONNECT:
		return "UI - connect";
	case UI_LISTEN:
		return "UI - listen";
	case UI_DISCONNECT:
		return "UI - disconnect";
	case UI_PREFERENCES:
		return "UI - preferences";
	case UI_START_GAME:
		return "UI - start game";
	case UI_PAUSE_GAME:
		return "UI - pause game";
	case UI_EXIT_GAME:
		return "UI - exit game";
	case UI_CLOSE_APPLICATION:
		return "UI - close application";
	case UI_CHAT_MESSAGE:
		return "UI - chat message";
	case FALCON_SLINGSHOT_POSITION:
		return "Falcon - slingshot position";
	case ZCAM_VIDEO_DATA:
		return "ZCam - video data";
	default:
		return "Unknown - log event";
	}
}

#endif
