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
	ReplayFormatLogger(std::auto_ptr<Stream> ostream);

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
	virtual void log(LogEvent_t logEvent, const PhysicsSync& sync);

private:
	ReplayFormatLogger(const ReplayFormatLogger& c); // intentionally not implemented
	ReplayFormatLogger& operator=(const ReplayFormatLogger& c); // intentionally not implemented

	long getElapsedTimeInMs() const;

	// This function is a helper for serializing the ReplayFormatEvent type
	// The serialization library doesn't like to serialize non-const types
	// so this creates and returns a const ReplayFormatEvent
	const ReplayFormatEvent toReplayEvent(LogEvent_t e) const;

	std::auto_ptr<Stream> ostream;
	const boost::posix_time::ptime start;

	boost::archive::text_oarchive archive;
	boost::mutex archive_mutex;
};

//---------------------------------------------
// Template Implementations
//---------------------------------------------

template <typename Stream>
ReplayFormatLogger<Stream>::ReplayFormatLogger(std::auto_ptr<Stream> stream) : 
	ostream(stream), 
	start(boost::posix_time::microsec_clock::local_time()),
	archive(*ostream)
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
void ReplayFormatLogger<Stream>::log(LogEvent_t e, const PhysicsSync& sync)
{
	boost::mutex::scoped_lock lock(archive_mutex);
	archive << toReplayEvent(e) << sync;
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
