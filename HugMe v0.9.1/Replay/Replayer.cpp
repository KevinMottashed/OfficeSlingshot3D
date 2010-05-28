#include "Replayer.h"
#include "ConsoleStream.h"

using namespace std;
using namespace boost;

Replayer::Replayer(const char* fileName, const UserPreferences& preferences) :
	MFCUserInterface(preferences),
	file(fileName, ios::in | ios::binary)
{
}

Replayer::~Replayer()
{
}

void Replayer::startReplay()
{
	replayThread = auto_ptr<thread>(new thread(boost::bind(&Replayer::replay, this)));
	return;
}

void Replayer::endReplay()
{
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

	bool open = file.is_open();

	// keep going until we reach the end of the file
	while (!file.eof())
	{
		long ms;
		LogEvent logEvent;
		unsigned int size;

		// extract the needed information from the file
		// each replay is formatted as <time><event><size><data>
		file.read((char*) &ms, sizeof(long));
		file.read((char*) &logEvent, sizeof(LogEvent));
		file.read((char*) &size, sizeof(unsigned int));

		// get the data
		scoped_array<BYTE> data(new BYTE[size]);
		file.read((char*) data.get(), size);

		if (!file.good())
		{
			// there was an error reading the file, exit
			break;
		}

		// only set the timer if there is a valid wait time
		if (ms > 0)
		{
			time_duration nextEvent = milliseconds(ms);
			time_duration current = microsec_clock::local_time() - startTime;
			time_duration wait = nextEvent - current;
			timer.expires_from_now(wait);
			timer.wait();
		}

		switch (logEvent)
		{
		case NETWORK_PEER_CONNECTED:
			{
				assert(size == 0);
				NetworkSubject::notify(PEER_CONNECTED);
				break;
			}
		case NETWORK_PEER_DISCONNECTED:
			{
				assert(size == 0);
				NetworkSubject::notify(PEER_DISCONNECTED);
				break;
			}
		case NETWORK_PEER_START_GAME:
			{
				assert(size == 0);
				NetworkSubject::notify(PEER_START_GAME);
				break;
			}
		case NETWORK_PEER_PAUSE_GAME:
			{
				assert(size == 0);
				NetworkSubject::notify(PEER_PAUSE_GAME);
				break;
			}
		case NETWORK_PEER_EXIT_GAME:
			{
				assert(size == 0);
				NetworkSubject::notify(PEER_EXIT_GAME);
				break;
			}
		case NETWORK_ERROR_OCCURED:
			{
				assert(size == sizeof(rc_network));
				rc_network code;
				memcpy(&code, data.get(), size);
				NetworkSubject::notify(NETWORK_ERROR, &code);
				break;
			}
		case NETWORK_USER_NAME:
			{
				string str((char*) data.get());
				NetworkSubject::notify(RECEIVED_USER_NAME, &str);
				break;
			}
		case NETWORK_CHAT_MESSAGE:
			{
				string str((char*) data.get());
				NetworkSubject::notify(RECEIVED_CHAT_MESSAGE, &str);
				break;
			}
		case NETWORK_VIDEO_DATA:
			{
				assert(size == IMAGE_ARRAY_SIZE);
				shared_ptr<vector<BYTE> > rgb(new vector<BYTE>(IMAGE_ARRAY_SIZE));
				memcpy(&rgb->front(), data.get(), size);
				VideoData video(rgb);
				NetworkSubject::notify(RECEIVED_VIDEO, &video);
				break;
			}
		case NETWORK_SLINGSHOT_POSITION:
			{
				assert(size == 3 * sizeof(double));
				cVector3d vec;
				memcpy(&vec.x, data.get(), sizeof(double));
				memcpy(&vec.y, data.get() + sizeof(double), sizeof(double));
				memcpy(&vec.z, data.get() + 2 * sizeof(double), sizeof(double));
				NetworkSubject::notify(RECEIVED_SLINGSHOT_POSITION, &vec);
				break;
			}
		case NETWORK_PROJECTILE:
			{
				assert(size == 6 * sizeof(double));
				
				cVector3d position;
				cVector3d speed;
				memcpy(&position.x, data.get() + 0 * sizeof(double), sizeof(double));
				memcpy(&position.y, data.get() + 1 * sizeof(double), sizeof(double));
				memcpy(&position.z, data.get() + 2 * sizeof(double), sizeof(double));
				memcpy(&speed.x, data.get() + 3 * sizeof(double), sizeof(double));
				memcpy(&speed.y, data.get() + 4 * sizeof(double), sizeof(double));
				memcpy(&speed.z, data.get() + 5 * sizeof(double), sizeof(double));

				Projectile p;
				p.setPosition(position);
				p.setSpeed(speed);
				NetworkSubject::notify(RECEIVED_PROJECTILE, &p);
				break;
			}
		case NETWORK_SLINGSHOT_PULLBACK:
			{
				assert(size == 0);
				NetworkSubject::notify(RECEIVED_PULLBACK);
				break;
			}
		case NETWORK_SLINGSHOT_RELEASE:
			{
				assert(size == 0);
				NetworkSubject::notify(RECEIVED_RELEASE);
				break;
			}
		case NETWORK_PLAYER_POSITION:
			{
				assert(size == 3 * sizeof(double));
				cVector3d vec;
				memcpy(&vec.x, data.get(), sizeof(double));
				memcpy(&vec.y, data.get() + sizeof(double), sizeof(double));
				memcpy(&vec.z, data.get() + 2 * sizeof(double), sizeof(double));
				NetworkSubject::notify(RECEIVED_PLAYER_POSITION, &vec);
				break;
			}
		case UI_CONNECT:
			{
				assert(size == 0);
				UserInterfaceSubject::notify(CONNECT);
				break;
			}
		case UI_LISTEN:
			{
				assert(size == 0);
				UserInterfaceSubject::notify(LISTEN);
				break;
			}
		case UI_DISCONNECT:
			{
				assert(size == 0);
				UserInterfaceSubject::notify(DISCONNECT);
				break;
			}
		case UI_PREFERENCES:
			{
				assert(size >= sizeof(unsigned int) * 4);
				UserPreferences preferences;
				unsigned int index = 0;

				preferences.ipAddress = (char*) data.get() + index;
				index += preferences.ipAddress.size();

				preferences.name = (char*) data.get() + index;
				index += preferences.name.size();

				memcpy(&preferences.armBandPort, data.get() + index, sizeof(unsigned int));
				index += sizeof(unsigned int);

				memcpy(&preferences.jacketPort, data.get() + index, sizeof(unsigned int));
				index += sizeof(unsigned int);

				UserInterfaceSubject::notify(PREFERENCES, &preferences);
				break;
			}
		case UI_START_GAME:
			{
				assert(size == 0);
				UserInterfaceSubject::notify(START_GAME);
				break;
			}
		case UI_PAUSE_GAME:
			{
				assert(size == 0);
				UserInterfaceSubject::notify(PAUSE_GAME);
				break;
			}
		case UI_EXIT_GAME:
			{
				assert(size == 0);
				UserInterfaceSubject::notify(EXIT_GAME);
				break;
			}
		case UI_CLOSE_APPLICATION:
			{
				assert(size == 0);
				UserInterfaceSubject::notify(EXIT_APPLICATION);

				// notify the replay watcher that the application would have been closed
				// by not closing the application we can inspect the UI, debug, etc...
				m_pMainDlg->MessageBox("APPLICATION CLOSED");
								
				break;
			}
		case UI_CHAT_MESSAGE:
			{
				string str((char*) data.get());

				// add the message to the UI
				m_pMainDlg->displayLocalChatMessage(str);

				UserInterfaceSubject::notify(CHAT_MESSAGE, &str);
				break;
			}
		case FALCON_SLINGSHOT_POSITION:
			{
				assert(size == 3 * sizeof(double));
				cVector3d vec;
				memcpy(&vec.x, data.get(), sizeof(double));
				memcpy(&vec.y, data.get() + sizeof(double), sizeof(double));
				memcpy(&vec.z, data.get() + 2 * sizeof(double), sizeof(double));
				FalconSubject::notify(SLINGSHOT_POSITION, &vec);
				break;
			}
		}		
	}
	return;
}

rc_network Replayer::listen(const std::string& userName)
{
	return SUCCESS;
}

rc_network Replayer::connect(const std::string& ipAddress, const std::string& userName)
{
	return SUCCESS;
}

rc_network Replayer::disconnect()
{
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

rc_network Replayer::sendVideoData(VideoData video)
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

void Replayer::start()
{
	return;
}

void Replayer::stop()
{
	return;
}
