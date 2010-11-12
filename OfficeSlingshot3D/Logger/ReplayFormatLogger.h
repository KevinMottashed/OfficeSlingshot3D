#ifndef REPLAY_FORMAT_LOGGER_H
#define REPLAY_FORMAT_LOGGER_H

#include "stdafx.h"
#include "boost.h"

#include "Logger.h"
#include "ConsoleStream.h"
#include "Projectile.h"
#include "ReplayFormatEvent.h"

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
	virtual void log(LogEvent_t logEvent);
	virtual void log(LogEvent_t logEvent, unsigned int uInt);
	virtual void log(LogEvent_t logEvent, rc_network error);
	virtual void log(LogEvent_t logEvent, const std::string& str);
	virtual void log(LogEvent_t logEvent, const cVector3d& vec);
	virtual void log(LogEvent_t logEvent, const Projectile& projectile);
	virtual void log(LogEvent_t logEvent, const UserPreferences& preferences);

private:
	ReplayFormatLogger(const ReplayFormatLogger& c); // intentionally not implemented
	ReplayFormatLogger& operator=(const ReplayFormatLogger& c); // intentionally not implemented

	long getElapsedTimeInMs() const;

	// This function is a helper for serializing the ReplayFormatEvent type
	// The serialization library doesn't like to serialize non-const types
	// so this creates and returns a const ReplayFormatEvent
	const ReplayFormatEvent toReplayEvent(LogEvent_t e) const;

	Stream ostream;
	const boost::posix_time::ptime start;

	boost::archive::text_oarchive archive;
	boost::mutex archive_mutex;
};

//---------------------------------------------
// Template Implementations
//---------------------------------------------

template <typename Stream>
ReplayFormatLogger<Stream>::ReplayFormatLogger() : 
	ostream(), 
	start(boost::posix_time::microsec_clock::local_time()),
	archive(ostream)
{
}

template <typename Stream>
template <typename T1>
ReplayFormatLogger<Stream>::ReplayFormatLogger(T1 param) : 
	ostream(param), 
	start(boost::posix_time::microsec_clock::local_time()),
	archive(ostream)
{
}

template <typename Stream>
template <typename T1, typename T2>
ReplayFormatLogger<Stream>::ReplayFormatLogger(T1 param1, T2 param2) : 
	ostream(param1, param2), 
	start(boost::posix_time::microsec_clock::local_time()),
	archive(ostream)
{
}

template <typename Stream>
template <typename T1, typename T2, typename T3>
ReplayFormatLogger<Stream>::ReplayFormatLogger(T1 param1, T2 param2, T3 param3) : 
	ostream(param1, param2, param3), 
	start(boost::posix_time::microsec_clock::local_time()),
	archive(ostream)
{
}


template <typename Stream>
ReplayFormatLogger<Stream>::~ReplayFormatLogger()
{
}

template <typename Stream>
void ReplayFormatLogger<Stream>::log(LogEvent_t e)
{
	boost::mutex::scoped_lock lock(archive_mutex);
	archive << toReplayEvent(e);
	return;
}

template <typename Stream>
void ReplayFormatLogger<Stream>::log(LogEvent_t e, unsigned int uInt)
{
	boost::mutex::scoped_lock lock(archive_mutex);
	archive << toReplayEvent(e) << uInt;
	return;
}

template <typename Stream>
void ReplayFormatLogger<Stream>::log(LogEvent_t e, rc_network error)
{
	boost::mutex::scoped_lock lock(archive_mutex);
	archive << toReplayEvent(e) << error;
	return;
}

template <typename Stream>
void ReplayFormatLogger<Stream>::log(LogEvent_t e, const std::string& str)
{
	boost::mutex::scoped_lock lock(archive_mutex);
	archive << toReplayEvent(e) << str;
	return;
}

template <typename Stream>
void ReplayFormatLogger<Stream>::log(LogEvent_t e, const cVector3d& vec)
{
	boost::mutex::scoped_lock lock(archive_mutex);
	archive << toReplayEvent(e) << vec;
	return;
}

template <typename Stream>
void ReplayFormatLogger<Stream>::log(LogEvent_t e, const Projectile& projectile)
{
	boost::mutex::scoped_lock lock(archive_mutex);
	archive << toReplayEvent(e) << projectile;
	return;
}

template <typename Stream>
void ReplayFormatLogger<Stream>::log(LogEvent_t e, const UserPreferences& preferences)
{
	boost::mutex::scoped_lock lock(archive_mutex);
	archive << toReplayEvent(e) << preferences;
	return;
}

template <typename Stream>
long ReplayFormatLogger<Stream>::getElapsedTimeInMs() const
{
	using namespace boost::posix_time;
	time_duration elapsed = microsec_clock::local_time() - start;
	return elapsed.total_milliseconds();
}

template <typename Stream>
const ReplayFormatEvent ReplayFormatLogger<Stream>::toReplayEvent(LogEvent_t e) const
{
	return ReplayFormatEvent(getElapsedTimeInMs(), e);
}

#endif
