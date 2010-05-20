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
