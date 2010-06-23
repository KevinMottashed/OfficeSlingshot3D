#include "Replayer.h"
#include "ConsoleStream.h"

using namespace std;
using namespace boost;

Replayer::Replayer(string fileName, const UserPreferences& preferences) :
	MFCUserInterface(preferences),

	// The file needs to be opened in binary mode to avoid any endline conversions
	file(fileName.c_str(), ios::in | ios::binary),

	archive(file),
	connectionState(ConnectionState::DISCONNECTED)
{
}

Replayer::~Replayer()
{
}

void Replayer::startReplay()
{
	// create a thread that will run the "replay()" function
	replayThread = auto_ptr<thread>(new thread(boost::bind(&Replayer::replay, this)));
	return;
}

void Replayer::stopReplay()
{
	// interrupt the thread
	replayThread->interrupt();
	return;
}

void Replayer::replay()
{
	using namespace boost::asio;
	using namespace boost::posix_time;

	// initialize the timer
	io_service io;
	deadline_timer timer(io);

	// get the start time
	startTime = microsec_clock::local_time();

	try
	{
		// keep going until we have an exception,
		// an exception will occur when we reach the end of the file
		while (true)
		{
			ReplayFormatEvent replayEvent;
			archive >> replayEvent;

			// only set the timer if there is a valid wait time
			if (replayEvent.time > 0)
			{
				time_duration nextEvent = milliseconds(replayEvent.time);
				time_duration current = microsec_clock::local_time() - startTime;
				time_duration wait = nextEvent - current;
				timer.expires_from_now(wait);
				timer.wait();
			}

			switch (replayEvent.logEvent)
			{
				case NETWORK_PEER_CONNECTED:
				{
					connectionState = ConnectionState::CONNECTED;
					NetworkSubject::notify(PEER_CONNECTED);
					break;
				}
				case NETWORK_PEER_DISCONNECTED:
				{
					connectionState = ConnectionState::DISCONNECTED;
					NetworkSubject::notify(PEER_DISCONNECTED);
					break;
				}
				case NETWORK_PEER_START_GAME:
				{
					NetworkSubject::notify(PEER_START_GAME);
					break;
				}
				case NETWORK_PEER_PAUSE_GAME:
				{
					NetworkSubject::notify(PEER_PAUSE_GAME);
					break;
				}
				case NETWORK_PEER_EXIT_GAME:
				{
					NetworkSubject::notify(PEER_EXIT_GAME);
					break;
				}
				case NETWORK_ERROR_OCCURED:
				{
					connectionState = ConnectionState::DISCONNECTED;

					rc_network code;
					archive >> code;

					NetworkSubject::notify(NETWORK_ERROR, &code);
					break;
				}
				case NETWORK_USER_NAME:
				{
					string str;
					archive >> str;

					NetworkSubject::notify(RECEIVED_USER_NAME, &str);
					break;
				}
				case NETWORK_CHAT_MESSAGE:
				{
					string str;
					archive >> str;

					NetworkSubject::notify(RECEIVED_CHAT_MESSAGE, &str);
					break;
				}
				case NETWORK_VIDEO_DATA:
				{
					VideoData video;
					archive >> video;

					NetworkSubject::notify(RECEIVED_VIDEO, &video);
					break;
				}
				case NETWORK_SLINGSHOT_POSITION:
				{
					cVector3d vec;
					archive >> vec;

					NetworkSubject::notify(RECEIVED_SLINGSHOT_POSITION, &vec);
					break;
				}
				case NETWORK_PROJECTILE:
				{
					Projectile p;
					archive >> p;

					NetworkSubject::notify(RECEIVED_PROJECTILE, &p);
					break;
				}
				case NETWORK_SLINGSHOT_PULLBACK:
				{
					NetworkSubject::notify(RECEIVED_PULLBACK);
					break;
				}
				case NETWORK_SLINGSHOT_RELEASE:
				{
					NetworkSubject::notify(RECEIVED_RELEASE);
					break;
				}
				case NETWORK_PLAYER_POSITION:
				{
					cVector3d vec;
					archive >> vec;

					NetworkSubject::notify(RECEIVED_PLAYER_POSITION, &vec);
					break;
				}
				case UI_CONNECT:
				{
					UserInterfaceSubject::notify(CONNECT);
					break;
				}
				case UI_LISTEN:
				{
					UserInterfaceSubject::notify(LISTEN);
					break;
				}
				case UI_DISCONNECT:
				{
					UserInterfaceSubject::notify(DISCONNECT);
					break;
				}
				case UI_PREFERENCES:
				{
					UserPreferences preferences;
					archive >> preferences;

					UserInterfaceSubject::notify(PREFERENCES, &preferences);
					break;
				}
				case UI_START_GAME:
				{
					UserInterfaceSubject::notify(START_GAME);
					break;
				}
				case UI_PAUSE_GAME:
				{
					UserInterfaceSubject::notify(PAUSE_GAME);
					break;
				}
				case UI_EXIT_GAME:
				{
					UserInterfaceSubject::notify(EXIT_GAME);
					break;
				}
				case UI_CLOSE_APPLICATION:
				{
					UserInterfaceSubject::notify(EXIT_APPLICATION);

					// notify the replay watcher that the application would have been closed
					// by not closing the application we can inspect the UI, debug, etc...
					m_pMainDlg->MessageBox("APPLICATION CLOSED");
									
					break;
				}
				case UI_CHAT_MESSAGE:
				{
					string str;
					archive >> str;

					// add the message to the UI
					m_pMainDlg->displayLocalChatMessage(str);

					UserInterfaceSubject::notify(CHAT_MESSAGE, &str);
					break;
				}
				case FALCON_SLINGSHOT_POSITION:
				{
					cVector3d vec;
					archive >> vec;

					FalconSubject::notify(SLINGSHOT_POSITION, &vec);
					break;
				}
				case ZCAM_VIDEO_DATA:
				{
					VideoData video;
					archive >> video;

					ZCameraSubject::notify(VIDEO, &video);
					break;
				}
			}		
		}
	}
	catch (boost::archive::archive_exception& e)
	{
		// the only exception that we are expecting is the stream_error one
		// this indicates an end of stream which will happen when the
		// file runs out of replay events, any other exception is
		// bad so rethrow it
		if (e.code != boost::archive::archive_exception::stream_error)
		{
			throw;
		}		
	}
	return;
}

rc_network Replayer::listen(const std::string& userName)
{
	connectionState = ConnectionState::LISTENING;
	return SUCCESS;
}

rc_network Replayer::connect(const std::string& ipAddress, const std::string& userName)
{
	connectionState = ConnectionState::CONNECTED;
	return SUCCESS;
}

rc_network Replayer::disconnect()
{
	connectionState = ConnectionState::DISCONNECTED;
	return SUCCESS;
}

rc_network Replayer::sendStartGame()
{
	return SUCCESS;
}

rc_network Replayer::sendPauseGame()
{
	return SUCCESS;
}

rc_network Replayer::sendEndGame()
{
	return SUCCESS;
}

rc_network Replayer::sendUserName(const std::string& userName)
{
	return SUCCESS;
}

rc_network Replayer::sendChatMessage(const std::string& message)
{
	return SUCCESS;
}

rc_network Replayer::sendVideoData(const VideoData& video)
{
	return SUCCESS;
}

rc_network Replayer::sendPlayerPosition(const cVector3d& position)
{
	return SUCCESS;
}

rc_network Replayer::sendSlingshotPosition(const cVector3d& position)
{
	return SUCCESS;
}

rc_network Replayer::sendProjectile(const Projectile& projectile)
{
	return SUCCESS;
}

rc_network Replayer::sendSlingshotPullback()
{
	return SUCCESS;
}

rc_network Replayer::sendSlingshotRelease()
{
	return SUCCESS;
}

bool Replayer::isConnected() const
{
	return connectionState == ConnectionState::CONNECTED;
}

bool Replayer::isListening() const
{
	return connectionState == ConnectionState::LISTENING;
}

void Replayer::startPolling()
{
	return;
}

void Replayer::stopPolling()
{
	return;
}

void Replayer::startCapture()
{
	return;
}

void Replayer::stopCapture()
{
	return;
}
