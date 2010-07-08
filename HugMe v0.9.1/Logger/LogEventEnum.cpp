#include "LogEventEnum.h"

// convert the enum to a string
const std::string LogEvent::lookup(LogEvent::LogEventEnum logEvent)
{
	switch(logEvent)
	{
	case LogEvent::NETWORK_PEER_CONNECTED:
		return "Network - peer connected";
	case LogEvent::NETWORK_PEER_DISCONNECTED:
		return "Network - peer disconnected";
	case LogEvent::NETWORK_PEER_START_GAME:
		return "Network - peer start game";
	case LogEvent::NETWORK_PEER_PAUSE_GAME:
		return "Network - peer pause game";
	case LogEvent:: NETWORK_PEER_EXIT_GAME:
		return "Network - peer exit game";
	case LogEvent::NETWORK_ERROR_OCCURED:
		return "Network - error occured";
	case LogEvent::NETWORK_USER_NAME:
		return "Network - user name";
	case LogEvent::NETWORK_CHAT_MESSAGE:
		return "Network - chat message";
	case LogEvent::NETWORK_VIDEO_DATA:
		return "Network - video data";
	case LogEvent::NETWORK_SLINGSHOT_POSITION:
		return "Network - slingshot position";
	case LogEvent::NETWORK_PROJECTILE:
		return "Network - projectile";
	case LogEvent::NETWORK_SLINGSHOT_PULLBACK:
		return "Network - slingshot pullback";
	case LogEvent::NETWORK_SLINGSHOT_RELEASE:
		return "Network - slingshot release";
	case LogEvent::NETWORK_PLAYER_POSITION:
		return "Network - player position";
	case LogEvent::UI_CONNECT:
		return "UI - connect";
	case LogEvent::UI_LISTEN:
		return "UI - listen";
	case LogEvent::UI_DISCONNECT:
		return "UI - disconnect";
	case LogEvent::UI_PREFERENCES:
		return "UI - preferences";
	case LogEvent::UI_START_GAME:
		return "UI - start game";
	case LogEvent::UI_PAUSE_GAME:
		return "UI - pause game";
	case LogEvent::UI_EXIT_GAME:
		return "UI - exit game";
	case LogEvent::UI_CLOSE_APPLICATION:
		return "UI - close application";
	case LogEvent::UI_CHAT_MESSAGE:
		return "UI - chat message";
	case LogEvent::FALCON_SLINGSHOT_POSITION:
		return "Falcon - slingshot position";
	case LogEvent::ZCAM_VIDEO_DATA:
		return "ZCam - video data";
	default:
		return "Unknown - log event";
	}
}

bool LogEvent::isNetworkEvent(LogEvent::LogEventEnum e)
{
	return e >= START_NETWORK && e < END_NETWORK;
}

bool LogEvent::isUIEvent(LogEvent::LogEventEnum e)
{
	return e >= START_UI && e < END_UI;
}

bool LogEvent::isFalconEvent(LogEvent::LogEventEnum e)
{
	return e >= START_FALCON && e < END_FALCON;	
}

bool LogEvent::isZCamEvent(LogEvent::LogEventEnum e)
{
	return e >= START_ZCAM && e < END_ZCAM;
}

