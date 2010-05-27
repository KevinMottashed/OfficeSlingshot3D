#include "Replayer.h"
#include "ConsoleStream.h"

using namespace std;
using namespace boost;

Replayer::Replayer(const char* fileName) : file(fileName, ios::in | ios::binary)
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
	start = microsec_clock::local_time();

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
			time_duration current = microsec_clock::local_time() - start;
			time_duration wait = nextEvent - current;
			timer.expires_from_now(wait);
			timer.wait();
		}

		switch (logEvent)
		{
		case NETWORK_PEER_CONNECTED:
			{
				assert(size == 0);
				notify(PEER_CONNECTED);
				break;
			}
		case NETWORK_PEER_DISCONNECTED:
			{
				assert(size == 0);
				notify(PEER_DISCONNECTED);
				break;
			}
		case NETWORK_PEER_START_GAME:
			{
				assert(size == 0);
				notify(PEER_START_GAME);
				break;
			}
		case NETWORK_PEER_PAUSE_GAME:
			{
				assert(size == 0);
				notify(PEER_PAUSE_GAME);
				break;
			}
		case NETWORK_PEER_EXIT_GAME:
			{
				assert(size == 0);
				notify(PEER_EXIT_GAME);
				break;
			}
		case NETWORK_ERROR_OCCURED:
			{
				assert(size == sizeof(rc_network));
				rc_network code;
				memcpy(&code, data.get(), size);
				notify(NETWORK_ERROR, &code);
				break;
			}
		case NETWORK_USER_NAME:
			{
				string str((char*) data.get());
				notify(RECEIVED_USER_NAME, &str);
				break;
			}
		case NETWORK_CHAT_MESSAGE:
			{
				string str((char*) data.get());
				notify(RECEIVED_CHAT_MESSAGE, &str);
				break;
			}
		case NETWORK_VIDEO_DATA:
			{
				assert(size == IMAGE_ARRAY_SIZE);
				shared_ptr<vector<BYTE> > rgb(new vector<BYTE>(IMAGE_ARRAY_SIZE));
				memcpy(&rgb->front(), data.get(), size);
				VideoData video(rgb);
				notify(RECEIVED_VIDEO, &video);
				break;
			}
		case NETWORK_SLINGSHOT_POSITION:
			{
				assert(size == 3 * sizeof(double));
				cVector3d vec;
				memcpy(&vec.x, data.get(), sizeof(double));
				memcpy(&vec.y, data.get() + sizeof(double), sizeof(double));
				memcpy(&vec.z, data.get() + 2 * sizeof(double), sizeof(double));
				notify(RECEIVED_SLINGSHOT_POSITION, &vec);
				break;
			}
		case NETWORK_PROJECTILE:
			{
				assert(size == 3 * sizeof(double));
				
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
				notify(RECEIVED_PROJECTILE, &p);
				break;
			}
		case NETWORK_SLINGSHOT_PULLBACK:
			{
				assert(size == 0);
				notify(RECEIVED_PULLBACK);
				break;
			}
		case NETWORK_SLINGSHOT_RELEASE:
			{
				assert(size == 0);
				notify(RECEIVED_RELEASE);
				break;
			}
		case NETWORK_PLAYER_POSITION:
			{
				assert(size == 3 * sizeof(double));
				cVector3d vec;
				memcpy(&vec.x, data.get(), sizeof(double));
				memcpy(&vec.y, data.get() + sizeof(double), sizeof(double));
				memcpy(&vec.z, data.get() + 2 * sizeof(double), sizeof(double));
				notify(RECEIVED_PLAYER_POSITION, &vec);
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
