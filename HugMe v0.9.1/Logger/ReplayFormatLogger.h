#ifndef REPLAY_FORMAT_LOGGER_H
#define REPLAY_FORMAT_LOGGER_H

#include "stdafx.h"
#include "boost.h"
#include "Logger.h"
#include "ConsoleStream.h"
#include "Projectile.h"

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
	virtual void log(LogEvent logEvent, rc_network error);
	virtual void log(LogEvent logEvent, const std::string& str);
	virtual void log(LogEvent logEvent, const VideoData& video);
	virtual void log(LogEvent logEvent, const cVector3d& vec);
	virtual void log(LogEvent logEvent, const Projectile& projectile);
	virtual void log(LogEvent logEvent, const UserPreferences& preferences);

private:
	ReplayFormatLogger(const ReplayFormatLogger& c); // intentionally not implemented
	ReplayFormatLogger& operator=(const ReplayFormatLogger& c); // intentionally not implemented

	long getElapsedTimeInMs() const;

	Stream ostream;
	const boost::posix_time::ptime start;
};

//---------------------------------------------
// Template Implementation
//---------------------------------------------

template <typename Stream>
ReplayFormatLogger<Stream>::ReplayFormatLogger() : 
	ostream(), 
	start(boost::posix_time::microsec_clock::local_time())
{
}

template <typename Stream>
template <typename T1>
ReplayFormatLogger<Stream>::ReplayFormatLogger(T1 param) : 
	ostream(param), 
	start(boost::posix_time::microsec_clock::local_time())
{
}

template <typename Stream>
template <typename T1, typename T2>
ReplayFormatLogger<Stream>::ReplayFormatLogger(T1 param1, T2 param2) : 
	ostream(param1, param2), 
	start(boost::posix_time::microsec_clock::local_time())
{
}

template <typename Stream>
template <typename T1, typename T2, typename T3>
ReplayFormatLogger<Stream>::ReplayFormatLogger(T1 param1, T2 param2, T3 param3) : 
	ostream(param1, param2, param3), 
	start(boost::posix_time::microsec_clock::local_time())
{
}


template <typename Stream>
ReplayFormatLogger<Stream>::~ReplayFormatLogger()
{
}

template <typename Stream>
void ReplayFormatLogger<Stream>::log(LogEvent e)
{
	long ms = getElapsedTimeInMs();
	unsigned int size = 0;

	// each event has the format <time><event><size><data>
	// write this event to the file
	ostream.write((char*) &ms, sizeof(long));
	ostream.write((char*) &e, sizeof(LogEvent));
	ostream.write((char*) &size, sizeof(unsigned int));
	ostream << std::flush;
	return;
}

template <typename Stream>
void ReplayFormatLogger<Stream>::log(LogEvent e, rc_network error)
{
	long ms = getElapsedTimeInMs();
	unsigned int size = sizeof(rc_network);

	// each event has the format <time><event><size><data>
	// write this event to the file
	ostream.write((char*) &ms, sizeof(long));
	ostream.write((char*) &e, sizeof(LogEvent));
	ostream.write((char*) &size, sizeof(unsigned int));
	ostream.write((char*) &error, sizeof(rc_network));
	ostream << std::flush;
	return;
}

template <typename Stream>
void ReplayFormatLogger<Stream>::log(LogEvent e, const std::string& str)
{
	long ms = getElapsedTimeInMs();

	// the +1 is for the null termination character
	unsigned int size = str.size() + 1;

	// each event has the format <time><event><size><data>
	// write this event to the file
	ostream.write((char*) &ms, sizeof(long));
	ostream.write((char*) &e, sizeof(LogEvent));
	ostream.write((char*) &size, sizeof(unsigned int));
	ostream.write((char*) str.c_str(), str.size() + 1);
	ostream << std::flush;
	return;
}

template <typename Stream>
void ReplayFormatLogger<Stream>::log(LogEvent e, const VideoData& video)
{
	long ms = getElapsedTimeInMs();
	unsigned int size = IMAGE_ARRAY_SIZE;

	// each event has the format <time><event><size><data>
	// write this event to the file
	ostream.write((char*) &ms, sizeof(long));
	ostream.write((char*) &e, sizeof(LogEvent));
	ostream.write((char*) &size, sizeof(unsigned int));
	ostream.write((char*) &video.rgb->front(), IMAGE_ARRAY_SIZE);
	ostream << std::flush;
	return;
}

template <typename Stream>
void ReplayFormatLogger<Stream>::log(LogEvent e, const cVector3d& vec)
{
	long ms = getElapsedTimeInMs();
	unsigned int size = 3 * sizeof(double);

	// each event has the format <time><event><size><data>
	// write this event to the file
	ostream.write((char*) &ms, sizeof(long));
	ostream.write((char*) &e, sizeof(LogEvent));
	ostream.write((char*) &size, sizeof(unsigned int));

	// the format of a cVector3d is <x><y><z>
	ostream.write((char*) &vec.x, sizeof(double));
	ostream.write((char*) &vec.y, sizeof(double));
	ostream.write((char*) &vec.z, sizeof(double));

	ostream << std::flush;
	return;
}

template <typename Stream>
void ReplayFormatLogger<Stream>::log(LogEvent e, const Projectile& projectile)
{
	long ms = getElapsedTimeInMs();
	unsigned int size = 6 * sizeof(double);

	// each event has the format <time><event><size><data>
	// write this event to the file
	ostream.write((char*) &ms, sizeof(long));
	ostream.write((char*) &e, sizeof(LogEvent));
	ostream.write((char*) &size, sizeof(unsigned int));

	// the format of a Projectile is <position><speed>
	// and the format of a cVector3d is <x><y><z>
	ostream.write((char*) &(projectile.getPosition().x), sizeof(double));
	ostream.write((char*) &(projectile.getPosition().y), sizeof(double));
	ostream.write((char*) &(projectile.getPosition().z), sizeof(double));
	ostream.write((char*) &(projectile.getSpeed().x), sizeof(double));
	ostream.write((char*) &(projectile.getSpeed().y), sizeof(double));
	ostream.write((char*) &(projectile.getSpeed().z), sizeof(double));

	ostream << std::flush;
	return;
}

template <typename Stream>
void ReplayFormatLogger<Stream>::log(LogEvent e, const UserPreferences& preferences)
{
	long ms = getElapsedTimeInMs();
	unsigned int size = 0;

	// each event has the format <time><event><size><data>
	// write this event to the file
	ostream.write((char*) &ms, sizeof(long));
	ostream.write((char*) &e, sizeof(LogEvent));
	ostream.write((char*) &size, sizeof(unsigned int));

	// user preferences are serialized as
	// <IP><\0><User name><\0><arm band port><jacket port>
	ostream.write(preferences.ipAddress.c_str(), preferences.ipAddress.size() + 1); // +1 for \0
	ostream.write(preferences.name.c_str(), preferences.name.size() + 1); // +1 for \0
	ostream.write((char*) &preferences.armBandPort, sizeof(unsigned int));
	ostream.write((char*) &preferences.jacketPort, sizeof(unsigned int));

	ostream << std::flush;
	return;
}

template <typename Stream>
long ReplayFormatLogger<Stream>::getElapsedTimeInMs() const
{
	using namespace boost::posix_time;
	time_duration elapsed = microsec_clock::local_time() - start;
	return elapsed.total_milliseconds();
}

#endif
