#include "Replayer.h"

using namespace std;
using namespace boost;

Replayer::Replayer(const string& fileName, const UserPreferences& preferences) :
	// The file needs to be opened in binary mode to avoid any endline conversions
	preferences(preferences),
	file(shared_ptr<ifstream>(new ifstream(fileName.c_str(), ios::in | ios::binary))),	
	archive(shared_ptr<archive::text_iarchive>(new archive::text_iarchive(*file)))
{
}

Replayer::~Replayer()
{
	// The archive must be destroyed before the file stream.
	// This is because the archive uses the file so if we destroy
	// the file first the archive will no longer have a file.
	archive.reset();
	file.reset();
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

void Replayer::initializeNetworkReplayer()
{
	if (networkReplayer)
	{
		// the network replayer is already initialized
		return;
	}
	networkReplayer = boost::shared_ptr<NetworkReplayer>(new NetworkReplayer(file, archive));
	return;
}

void Replayer::initializeUserInterfaceReplayer()
{
	if (uiReplayer)
	{
		// the ui replayer is already initialized
		return;		
	}
	uiReplayer = boost::shared_ptr<UserInterfaceReplayer>(new UserInterfaceReplayer(preferences, file, archive));
	return;
}

void Replayer::initializeFalconReplayer()
{
	if (falconReplayer)
	{
		// the falcon replayers
		return;
	}
	falconReplayer = boost::shared_ptr<FalconReplayer>(new FalconReplayer(file, archive));
	return;	
}

void Replayer::removeNetworkReplayer()
{
	networkReplayer.reset();
	return;
}

void Replayer::removeUserInterfaceReplayer()
{
	uiReplayer.reset();
	return;
}

void Replayer::removeFalconReplayer()
{
	falconReplayer.reset();
	return;
} 

shared_ptr<NetworkReplayer> Replayer::getNetworkReplayer()
{
	return networkReplayer;
}

shared_ptr<UserInterfaceReplayer> Replayer::getUserInterfaceReplayer()
{
	return uiReplayer;
}

shared_ptr<FalconReplayer> Replayer::getFalconReplayer()
{
	return falconReplayer;
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
			*archive >> replayEvent;

			// only set the timer if there is a valid wait time
			if (replayEvent.time > 0)
			{
				time_duration nextEvent = milliseconds(replayEvent.time);
				time_duration current = microsec_clock::local_time() - startTime;
				time_duration wait = nextEvent - current;
				timer.expires_from_now(wait);
				timer.wait();
			}

			if (LogEvent::isNetworkEvent(replayEvent.logEvent))
			{
				if (networkReplayer)
				{
					networkReplayer->replay(replayEvent.logEvent);
				}
				continue;
			}

			if (LogEvent::isUIEvent(replayEvent.logEvent))
			{
				if (uiReplayer)
				{
					uiReplayer->replay(replayEvent.logEvent);
				}
				continue;
			}

			if (LogEvent::isFalconEvent(replayEvent.logEvent))
			{
				if (falconReplayer)
				{
					falconReplayer->replay(replayEvent.logEvent);
				}
				continue;
			}

			switch (replayEvent.logEvent)
			{
				case LogEvent::ZCAM_VIDEO_DATA:
				{
					VideoData video;
					*archive >> video;

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

void Replayer::startCapture()
{
	return;
}

void Replayer::stopCapture()
{
	return;
}
