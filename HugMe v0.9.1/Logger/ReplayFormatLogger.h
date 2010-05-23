#ifndef REPLAY_FORMAT_LOGGER_H
#define REPLAY_FORMAT_LOGGER_H

#include "stdafx.h"
#include "boost.h"
#include "Logger.h"
#include "ConsoleStream.h"

// concrete logger class
// this logger outputs the log in the replay format
// Stream is the stream type
template <typename Stream>
class ReplayFormatLogger : public Logger
{
public:
	// Constructors
	// many stream types cannot be copied (including the STL ones)
	// so we must provide templated constructors to be able to initialize the stream
	ReplayFormatLogger();

	template <typename T1>
	ReplayFormatLogger(T1 param);

	template <typename T1, typename T2>
	ReplayFormatLogger(T1 param1, T2 param2);

	template <typename T1, typename T2, typename T3>
	ReplayFormatLogger(T1 param1, T2 param2, T3 param3);

	virtual ~ReplayFormatLogger();

protected:
	// log various event of various data types
	virtual void log(LogEvent logEvent);
	virtual void log(LogEvent logEvent, rc_network error) {};
	virtual void log(LogEvent logEvent, const std::string& str) {};
	virtual void log(LogEvent logEvent, const VideoData& video) {};
	virtual void log(LogEvent logEvent, const cVector3d& vec) {};
	virtual void log(LogEvent logEvent, const Projectile& projectile) {};
	virtual void log(LogEvent logEvent, const UserPreferences& preferences) {};

private:
	ReplayFormatLogger(const ReplayFormatLogger& c); // intentionally not implemented
	ReplayFormatLogger& operator=(const ReplayFormatLogger& c); // intentionally not implemented

	Stream ostream;
	const boost::posix_time::ptime start;
};

//---------------------------------------------
// Template Implementation
//---------------------------------------------

template <typename Stream>
ReplayFormatLogger<Stream>::ReplayFormatLogger() : ostream()
{
}

template <typename Stream>
template <typename T1>
ReplayFormatLogger<Stream>::ReplayFormatLogger(T1 param) : ostream(param)
{
}

template <typename Stream>
template <typename T1, typename T2>
ReplayFormatLogger<Stream>::ReplayFormatLogger(T1 param1, T2 param2) : ostream(param1, param2)
{
}

template <typename Stream>
template <typename T1, typename T2, typename T3>
ReplayFormatLogger<Stream>::ReplayFormatLogger(T1 param1, T2 param2, T3 param3) : ostream(param1, param2, param3)
{
}


template <typename Stream>
ReplayFormatLogger<Stream>::~ReplayFormatLogger()
{
}

template <typename Stream>
void ReplayFormatLogger<Stream>::log(LogEvent e)
{
	using namespace boost::posix_time;
	time_duration elapsed = microsec_clock::local_time() - start;
	long ms = elapsed.total_milliseconds();
	unsigned int size = 0;

	// each event has the format <time><event><size><data>
	// write this event to the file
	ostream.write((char*) &ms, sizeof(long));
	ostream.write((char*) &e, sizeof(LogEvent));
	ostream.write((char*) &size, sizeof(unsigned int));
	ostream << std::flush;
	return;
}

#endif
