#include "Replayer.h"

using namespace std;
using namespace boost;

Replayer::Replayer(const string& fileName, const UserPreferences& preferences) :
	preferences(preferences),
	// The file needs to be opened in binary mode to avoid any endline conversions
	file(shared_ptr<ifstream>(new ifstream(fileName.c_str(), ios::in | ios::binary))),	
	archive(shared_ptr<archive::text_iarchive>(new archive::text_iarchive(*file))),
	networkReplayer(new NetworkReplayer(file, archive)),
	uiReplayer(new UserInterfaceReplayer(preferences, file, archive)),
	falconReplayer(new FalconReplayer(file, archive)),
	zCameraReplayer(new ZCameraReplayer(file, archive))
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

shared_ptr<ZCameraReplayer> Replayer::getZCameraReplayer()
{
	return zCameraReplayer;
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
				networkReplayer->replay(replayEvent.logEvent);
				continue;
			}

			if (LogEvent::isUIEvent(replayEvent.logEvent))
			{
				uiReplayer->replay(replayEvent.logEvent);
				continue;
			}

			if (LogEvent::isFalconEvent(replayEvent.logEvent))
			{
				falconReplayer->replay(replayEvent.logEvent);
				continue;
			}

			if (LogEvent::isZCamEvent(replayEvent.logEvent))
			{
				zCameraReplayer->replay(replayEvent.logEvent);
				continue;
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

