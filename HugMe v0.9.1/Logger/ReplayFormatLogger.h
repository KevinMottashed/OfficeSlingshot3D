#ifndef REPLAY_FORMAT_LOGGER_H
#define REPLAY_FORMAT_LOGGER_H

#include "stdafx.h"
#include "boost.h"
#include "Logger.h"
#include "ConsoleStream.h"

enum ReplayEvent
{
	NETWORK_PEER_CONNECTED,
	NETWORK_PEER_DISCONNECTED
};

// concrete logger class
// this logger outputs the log in a human readable format
// S is the stream type
// T1 is the type needed to initialize the stream
template <typename S, typename T1 = int, typename T2 = int>
class ReplayFormatLogger : public Logger
{
public:
	ReplayFormatLogger();
	ReplayFormatLogger(T1 param);
	ReplayFormatLogger(T1 param1, T2 param2);
	virtual ~ReplayFormatLogger();

protected:
	// log network events
	virtual void logPeerConnected();
	virtual void logPeerDisconnected();
	virtual void logPeerStartGame() {};
	virtual void logPeerPauseGame() {};
	virtual void logPeerExitGame() {};
	virtual void logNetworkError(rc_network error) {};
	virtual void logUserNameReceived(const std::string& name) {};
	virtual void logChatMessageReceived(const std::string& message) {};
	virtual void logVideoDataReceived(const VideoData& data) {};
	virtual void logSlingshotPositionReceived(const cVector3d& position) {};
	virtual void logProjectileReceived(const Projectile& projectile) {};
	virtual void logPullbackReceived() {};
	virtual void logReleaseReceived() {};
	virtual void logPlayerPositionReceived(const cVector3d& position) {};

	// log user interface events
	virtual void logConnect() {};
	virtual void logListen() {};
	virtual void logDisconnect() {};
	virtual void logChangePreferences(const UserPreferences& preferences) {};
	virtual void logLocalStartGame() {};
	virtual void logLocalPauseGame() {};
	virtual void logLocalExitGame() {};
	virtual void logCloseApplication() {};
	virtual void logSendChatMessage(const std::string& message) {};

	// log falcon events
	virtual void logLocalSlingshotPosition(const cVector3d& position) {};

	// log zcam events
	virtual void logLocalVideoData(const VideoData& data) {};

private:
	ReplayFormatLogger(const ReplayFormatLogger& c); // intentionally not implemented
	ReplayFormatLogger& operator=(const ReplayFormatLogger& c); // intentionally not implemented

	void log(ReplayEvent e);

	template <typename T>
	void log(ReplayEvent e, const T& t);

	S ostream;
	const boost::posix_time::ptime start;
};

//---------------------------------------------
// Template Implementation
//---------------------------------------------

template <typename S, typename T1, typename T2>
ReplayFormatLogger<S,T1,T2>::ReplayFormatLogger() : ostream(), start(boost::posix_time::microsec_clock::local_time())
{
}

template <typename S, typename T1, typename T2>
ReplayFormatLogger<S,T1,T2>::ReplayFormatLogger(T1 param) : ostream(param), start(boost::posix_time::microsec_clock::local_time())
{
}

template <typename S, typename T1, typename T2>
ReplayFormatLogger<S,T1,T2>::ReplayFormatLogger(T1 param1, T2 param2) : ostream(param1, param2), start(boost::posix_time::microsec_clock::local_time())
{
}

template <typename S, typename T1, typename T2>
ReplayFormatLogger<S,T1,T2>::~ReplayFormatLogger()
{
}

template <typename S, typename T1, typename T2>
void ReplayFormatLogger<S,T1,T2>::logPeerConnected()
{
	log(NETWORK_PEER_CONNECTED);
	return;
}

template <typename S, typename T1, typename T2>
void ReplayFormatLogger<S,T1,T2>::logPeerDisconnected()
{
	log(NETWORK_PEER_DISCONNECTED);
	return;
}

template <typename S, typename T1, typename T2>
void ReplayFormatLogger<S,T1,T2>::log(ReplayEvent e)
{
	using namespace boost::posix_time;
	time_duration elapsed = microsec_clock::local_time() - start;
	long ms = elapsed.total_milliseconds();
	unsigned int size = 0;

	// each event has the format <time><event><size><data>
	// write this event to the file
	ostream.write((char*) &ms, sizeof(long));
	ostream.write((char*) &e, sizeof(ReplayEvent));
	ostream.write((char*) &size, sizeof(unsigned int));
	ostream << std::flush;
	return;
}

#endif
