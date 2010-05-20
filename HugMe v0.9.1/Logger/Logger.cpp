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
			log(NETWORK_PEER_CONNECTED);
			break;
		}
		case PEER_DISCONNECTED:
		{
			log(NETWORK_PEER_DISCONNECTED);
			break;
		}
		case PEER_START_GAME:
		{
			log(NETWORK_PEER_START_GAME);
			break;
		}
		case PEER_PAUSE_GAME:
		{
			log(NETWORK_PEER_PAUSE_GAME);
			break;
		}
		case PEER_EXIT_GAME:
		{
			log(NETWORK_PEER_EXIT_GAME);
			break;
		}
		case NETWORK_ERROR:
		{
			assert(data != NULL);
			log(NETWORK_ERROR_OCCURED, *(rc_network*) data);
			break;
		}
		case RECEIVED_USER_NAME:
		{
			assert(data != NULL);
			log(NETWORK_USER_NAME, *(const std::string*) data);
			break;
		}
		case RECEIVED_CHAT_MESSAGE:
		{
			assert(data != NULL);
			log(NETWORK_CHAT_MESSAGE, *(const std::string*) data);
			break;
		}
		case RECEIVED_VIDEO:
		{
			assert(data != NULL);
			log(NETWORK_VIDEO_DATA, *(VideoData*) data);
			break;
		}
		case RECEIVED_SLINGSHOT_POSITION:
		{
			assert(data != NULL);
			log(NETWORK_SLINGSHOT_POSITION, *(cVector3d*) data);
			break;
		}
		case RECEIVED_PROJECTILE:
		{
			assert(data != NULL);
			log(NETWORK_PROJECTILE, *(Projectile*) data);
			break;
		}
		case RECEIVED_PULLBACK:
		{			
			log(NETWORK_SLINGSHOT_PULLBACK);
			break;
		}
		case RECEIVED_RELEASE:
		{
			log(NETWORK_SLINGSHOT_RELEASE);
			break;
		}
		case RECEIVED_PLAYER_POSITION:
		{
			assert(data != NULL);
			log(NETWORK_PLAYER_POSITION, *(cVector3d*) data);
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
			log(UI_CONNECT);
			break;
		}
		case LISTEN:
		{
			log(UI_LISTEN);
			break;
		}
		case DISCONNECT:
		{
			log(UI_DISCONNECT);
			break;
		}
		case PREFERENCES:
		{
			assert(data != NULL);
			log(UI_PREFERENCES, *(UserPreferences*) data);
			break;
		}
		case START_GAME:
		{
			log(UI_START_GAME);
			break;
		}
		case PAUSE_GAME:
		{
			log(UI_PAUSE_GAME);
			break;
		}
		case EXIT_GAME:
		{
			log(UI_EXIT_GAME);
			break;
		}
		case EXIT_APPLICATION:
		{
			log(UI_CLOSE_APPLICATION);
			break;
		}
		case CHAT_MESSAGE:
		{
			assert(data != NULL);
			log(UI_CHAT_MESSAGE, *(std::string*) data);
			break;
		}
	}
	return;
}

void Logger::update(FalconUpdateContext context, const void* data)
{
	switch(context)
	{
		case SLINGSHOT_POSITION:
		{
			assert(data != NULL);
			log(FALCON_SLINGSHOT_POSITION, *(cVector3d*) data);
			break;
		}
	}
	return;
}

void Logger::update(GameUpdateContext context, const void* data)
{
	// no game events to log, for now
	return;
}

void Logger::update(ZCameraUpdateContext context, const void* data)
{
	switch(context)
	{
		case VIDEO:
		{
			assert(data != NULL);
			log(ZCAM_VIDEO_DATA, *(VideoData*) data);
			break;
		}
	}
	return;
}

