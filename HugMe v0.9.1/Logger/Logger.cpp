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
			logPeerConnected();
			break;
		}
		case PEER_DISCONNECTED:
		{
			logPeerDisconnected();
			break;
		}
		case PEER_START_GAME:
		{
			logPeerStartGame();
			break;
		}
		case PEER_PAUSE_GAME:
		{
			logPeerPauseGame();
			break;
		}
		case PEER_EXIT_GAME:
		{
			logPeerExitGame();
			break;
		}
		case NETWORK_ERROR:
		{
			assert(data != NULL);
			logNetworkError(*(rc_network*) data);
			break;
		}
		case RECEIVED_USER_NAME:
		{
			assert(data != NULL);
			logUserNameReceived(*(const std::string*) data);
			break;
		}
		case RECEIVED_CHAT_MESSAGE:
		{
			assert(data != NULL);
			logChatMessageReceived(*(const std::string*) data);
			break;
		}
		case RECEIVED_VIDEO:
		{
			assert(data != NULL);
			logVideoDataReceived(*(VideoData*) data);
			break;
		}
		case RECEIVED_SLINGSHOT_POSITION:
		{
			assert(data != NULL);
			logSlingshotPositionReceived(*(cVector3d*) data);
			break;
		}
		case RECEIVED_PROJECTILE:
		{
			assert(data != NULL);
			logProjectileReceived(*(Projectile*) data);
			break;
		}
		case RECEIVED_PULLBACK:
		{			
			logPullbackReceived();
			break;
		}
		case RECEIVED_RELEASE:
		{
			logReleaseReceived();
			break;
		}
		case RECEIVED_PLAYER_POSITION:
		{
			assert(data != NULL);
			logPlayerPositionReceived(*(cVector3d*) data);
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
			logConnect();
			break;
		}
		case LISTEN:
		{
			logListen();
			break;
		}
		case DISCONNECT:
		{
			logDisconnect();
			break;
		}
		case PREFERENCES:
		{
			assert(data != NULL);
			logChangePreferences(*(UserPreferences*) data);
			break;
		}
		case START_GAME:
		{
			logLocalStartGame();
			break;
		}
		case PAUSE_GAME:
		{
			logLocalPauseGame();
			break;
		}
		case EXIT_GAME:
		{
			logLocalExitGame();
			break;
		}
		case EXIT_APPLICATION:
		{
			logCloseApplication();
			break;
		}
		case CHAT_MESSAGE:
		{
			assert(data != NULL);
			logSendChatMessage(*(std::string*) data);
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
			logLocalSlingshotPosition(*(cVector3d*) data);
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
			logLocalVideoData(*(VideoData*) data);
			break;
		}
	}
	return;
}

