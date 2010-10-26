#include "NetworkReplayer.h"

NetworkReplayer::NetworkReplayer(	boost::shared_ptr<std::ifstream> file,
									boost::shared_ptr<boost::archive::text_iarchive> archive) :
	file(file),
	archive(archive)
{
}

NetworkReplayer::~NetworkReplayer()
{
	// The archive must be destroyed before the file stream.
	// This is because the archive uses the file so if we destroy
	// the file first the archive will no longer have a file.
	archive.reset();
	file.reset();
}

void NetworkReplayer::replay(LogEvent_t logEvent)
{
	// don't try handling an event that isnt a network event
	assert(LogEventCategory::lookupCategory(logEvent) == LogEventCategory::NETWORK);

	switch (logEvent)
	{
		case LogEvent::NETWORK_PEER_CONNECTED:
		{
			connectionState = ConnectionState::CONNECTED;
			notify(PEER_CONNECTED);
			break;
		}
		case LogEvent::NETWORK_PEER_DISCONNECTED:
		{
			connectionState = ConnectionState::DISCONNECTED;
			notify(PEER_DISCONNECTED);
			break;
		}
		case LogEvent::NETWORK_PEER_START_GAME:
		{
			notify(PEER_START_GAME);
			break;
		}
		case LogEvent::NETWORK_PEER_PAUSE_GAME:
		{
			notify(PEER_PAUSE_GAME);
			break;
		}
		case LogEvent::NETWORK_PEER_EXIT_GAME:
		{
			notify(PEER_EXIT_GAME);
			break;
		}
		case LogEvent::NETWORK_ERROR_OCCURED:
		{
			connectionState = ConnectionState::DISCONNECTED;

			rc_network code;
			*archive >> code;

			notify(NETWORK_ERROR, &code);
			break;
		}
		case LogEvent::NETWORK_USER_NAME:
		{
			string str;
			*archive >> str;

			notify(RECEIVED_USER_NAME, &str);
			break;
		}
		case LogEvent::NETWORK_CHAT_MESSAGE:
		{
			string str;
			*archive >> str;

			notify(RECEIVED_CHAT_MESSAGE, &str);
			break;
		}
		case LogEvent::NETWORK_VIDEO_DATA:
		{
			VideoData video;
			*archive >> video;

			notify(RECEIVED_VIDEO, &video);
			break;
		}
		case LogEvent::NETWORK_SLINGSHOT_POSITION:
		{
			cVector3d vec;
			*archive >> vec;

			notify(RECEIVED_SLINGSHOT_POSITION, &vec);
			break;
		}
		case LogEvent::NETWORK_PROJECTILE:
		{
			Projectile p;
			*archive >> p;

			notify(RECEIVED_PROJECTILE, &p);
			break;
		}
		case LogEvent::NETWORK_SLINGSHOT_PULLBACK:
		{
			notify(RECEIVED_PULLBACK);
			break;
		}
		case LogEvent::NETWORK_PLAYER_POSITION:
		{
			cVector3d vec;
			*archive >> vec;

			notify(RECEIVED_PLAYER_POSITION, &vec);
			break;
		}
	}
	return;
}

rc_network NetworkReplayer::listen(const std::string& userName)
{
	connectionState = ConnectionState::LISTENING;
	return SUCCESS;
}

rc_network NetworkReplayer::connect(const std::string& ipAddress, const std::string& userName)
{
	connectionState = ConnectionState::CONNECTED;
	return SUCCESS;
}

rc_network NetworkReplayer::disconnect()
{
	connectionState = ConnectionState::DISCONNECTED;
	return SUCCESS;
}

rc_network NetworkReplayer::sendStartGame()
{
	return SUCCESS;
}

rc_network NetworkReplayer::sendPauseGame()
{
	return SUCCESS;
}

rc_network NetworkReplayer::sendEndGame()
{
	return SUCCESS;
}

rc_network NetworkReplayer::sendUserName(const std::string& userName)
{
	return SUCCESS;
}

rc_network NetworkReplayer::sendChatMessage(const std::string& message)
{
	return SUCCESS;
}

rc_network NetworkReplayer::sendVideoData(const VideoData& video)
{
	return SUCCESS;
}

rc_network NetworkReplayer::sendPlayerPosition(const cVector3d& position)
{
	return SUCCESS;
}

rc_network NetworkReplayer::sendSlingshotPosition(const cVector3d& position)
{
	return SUCCESS;
}

rc_network NetworkReplayer::sendProjectile(const Projectile& projectile)
{
	return SUCCESS;
}

rc_network NetworkReplayer::sendSlingshotPullback()
{
	return SUCCESS;
}

rc_network NetworkReplayer::sendSlingshotRelease()
{
	return SUCCESS;
}

rc_network NetworkReplayer::sendGameOver()
{
	return SUCCESS;
}

bool NetworkReplayer::isConnected() const
{
	return connectionState == ConnectionState::CONNECTED;
}

bool NetworkReplayer::isListening() const
{
	return connectionState == ConnectionState::LISTENING;
}

