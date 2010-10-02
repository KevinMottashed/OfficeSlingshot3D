#include "Logger.h"

Logger::Logger()
{
}

Logger::~Logger()
{
}

void Logger::update(NetworkUpdateContext context, const void *data)
{
	switch (context)
	{
		case PEER_CONNECTED:
		{
			log(LogEvent::NETWORK_PEER_CONNECTED);
			break;
		}
		case PEER_DISCONNECTED:
		{
			log(LogEvent::NETWORK_PEER_DISCONNECTED);
			break;
		}
		case PEER_START_GAME:
		{
			log(LogEvent::NETWORK_PEER_START_GAME);
			break;
		}
		case PEER_PAUSE_GAME:
		{
			log(LogEvent::NETWORK_PEER_PAUSE_GAME);
			break;
		}
		case PEER_EXIT_GAME:
		{
			log(LogEvent::NETWORK_PEER_EXIT_GAME);
			break;
		}
		case NETWORK_ERROR:
		{
			assert(data != NULL);
			log(LogEvent::NETWORK_ERROR_OCCURED, *(rc_network*) data);
			break;
		}
		case RECEIVED_USER_NAME:
		{
			assert(data != NULL);
			log(LogEvent::NETWORK_USER_NAME, *(const std::string*) data);
			break;
		}
		case RECEIVED_CHAT_MESSAGE:
		{
			assert(data != NULL);
			log(LogEvent::NETWORK_CHAT_MESSAGE, *(const std::string*) data);
			break;
		}
		case RECEIVED_VIDEO:
		{
			assert(data != NULL);
			log(LogEvent::NETWORK_VIDEO_DATA, *(VideoData*) data);
			break;
		}
		case RECEIVED_SLINGSHOT_POSITION:
		{
			assert(data != NULL);
			log(LogEvent::NETWORK_SLINGSHOT_POSITION, *(cVector3d*) data);
			break;
		}
		case RECEIVED_PROJECTILE:
		{
			assert(data != NULL);
			log(LogEvent::NETWORK_PROJECTILE, *(Projectile*) data);
			break;
		}
		case RECEIVED_PULLBACK:
		{			
			log(LogEvent::NETWORK_SLINGSHOT_PULLBACK);
			break;
		}
		case RECEIVED_PLAYER_POSITION:
		{
			assert(data != NULL);
			log(LogEvent::NETWORK_PLAYER_POSITION, *(cVector3d*) data);
			break;
		}
	}
	return;
}

void Logger::update(UserInterfaceUpdateContext context, const void* data)
{
	switch (context)
	{
		case CONNECT:
		{
			log(LogEvent::UI_CONNECT);
			break;
		}
		case LISTEN:
		{
			log(LogEvent::UI_LISTEN);
			break;
		}
		case DISCONNECT:
		{
			log(LogEvent::UI_DISCONNECT);
			break;
		}
		case PREFERENCES:
		{
			assert(data != NULL);
			log(LogEvent::UI_PREFERENCES, *(UserPreferences*) data);
			break;
		}
		case START_GAME:
		{
			log(LogEvent::UI_START_GAME);
			break;
		}
		case PAUSE_GAME:
		{
			log(LogEvent::UI_PAUSE_GAME);
			break;
		}
		case EXIT_GAME:
		{
			log(LogEvent::UI_EXIT_GAME);
			break;
		}
		case EXIT_APPLICATION:
		{
			log(LogEvent::UI_CLOSE_APPLICATION);
			break;
		}
		case CHAT_MESSAGE:
		{
			assert(data != NULL);
			log(LogEvent::UI_CHAT_MESSAGE, *(std::string*) data);
			break;
		}
	}
	return;
}

void Logger::update(FalconUpdateContext context, const void* data)
{
	switch(context)
	{
		case SLINGSHOT_MOVED:
		{
			assert(data != NULL);
			log(LogEvent::FALCON_SLINGSHOT_POSITION, *(cVector3d*) data);
			break;
		}
	}
	return;
}

void Logger::update(ZCameraUpdateContext context, const void* data)
{
	switch(context)
	{
		case VIDEO:
		{
			assert(data != NULL);
			log(LogEvent::ZCAM_VIDEO_DATA, *(VideoData*) data);
			break;
		}
	}
	return;
}

