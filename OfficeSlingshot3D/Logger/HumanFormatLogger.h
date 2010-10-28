#ifndef HUMAN_FORMAT_LOGGER_H
#define HUMAN_FORMAT_LOGGER_H

#include "stdafx.h"
#include "boost.h"
#include "Logger.h"
#include "ConsoleStream.h"

// concrete logger class
// this logger outputs the log in a human readable format
// Stream is the stream type
template <typename Stream>
class HumanFormatLogger : public Logger
{
public:
	// Constructors
	// many stream types cannot be copied (including the STL ones)
	// so we must provide templated constructors to be able to initialize the stream
	HumanFormatLogger();

	template <typename T1>
	HumanFormatLogger(T1 param);

	template <typename T1, typename T2>
	HumanFormatLogger(T1 param1, T2 param2);

	template <typename T1, typename T2, typename T3>
	HumanFormatLogger(T1 param1, T2 param2, T3 param3);


	virtual ~HumanFormatLogger();

protected:
	// log various event of various data types
	virtual void log(LogEvent_t logEvent);
	virtual void log(LogEvent_t logEvent, unsigned int uInt);
	virtual void log(LogEvent_t logEvent, rc_network error);
	virtual void log(LogEvent_t logEvent, const std::string& str);
	virtual void log(LogEvent_t logEvent, const VideoData& video);
	virtual void log(LogEvent_t logEvent, const cVector3d& vec);
	virtual void log(LogEvent_t logEvent, const Projectile& projectile);
	virtual void log(LogEvent_t logEvent, const UserPreferences& preferences);

private:
	HumanFormatLogger(const HumanFormatLogger& c); // intentionally not implemented
	HumanFormatLogger& operator=(const HumanFormatLogger& c); // intentionally not implemented

	Stream ostream;
	boost::mutex stream_mutex;
};

//---------------------------------------------
// Template Implementation
//---------------------------------------------

template <typename Stream>
HumanFormatLogger<Stream>::HumanFormatLogger() : ostream()
{
}

template <typename Stream>
template <typename T1>
HumanFormatLogger<Stream>::HumanFormatLogger(T1 param) : ostream(param)
{
}

template <typename Stream>
template <typename T1, typename T2>
HumanFormatLogger<Stream>::HumanFormatLogger(T1 param1, T2 param2) : ostream(param1, param2)
{
}

template <typename Stream>
template <typename T1, typename T2, typename T3>
HumanFormatLogger<Stream>::HumanFormatLogger(T1 param1, T2 param2, T3 param3) : ostream(param1, param2, param3)
{
}

template <typename Stream>
HumanFormatLogger<Stream>::~HumanFormatLogger()
{
}

template <typename Stream>
void HumanFormatLogger<Stream>::log(LogEvent_t logEvent)
{
	boost::mutex::scoped_lock lock(stream_mutex);
	ostream << lookup(logEvent) << std::endl;
	return;
}

template <typename Stream>
void HumanFormatLogger<Stream>::log(LogEvent_t logEvent, unsigned int uInt)
{
	boost::mutex::scoped_lock lock(stream_mutex);
	ostream << lookup(logEvent) << " - " << uInt << std::endl;
	return;
}

template <typename Stream>
void HumanFormatLogger<Stream>::log(LogEvent_t logEvent, rc_network error)
{
	boost::mutex::scoped_lock lock(stream_mutex);
	ostream << lookup(logEvent) << " - " << lookup(error) << std::endl;
	return;
}

template <typename Stream>
void HumanFormatLogger<Stream>::log(LogEvent_t logEvent, const std::string& str)
{
	boost::mutex::scoped_lock lock(stream_mutex);
	ostream << lookup(logEvent) << " - " << str << std::endl;
	return;
}

template <typename Stream>
void HumanFormatLogger<Stream>::log(LogEvent_t logEvent, const VideoData& video)
{
	boost::mutex::scoped_lock lock(stream_mutex);
	ostream << lookup(logEvent) << std::endl;
	return;
}

template <typename Stream>
void HumanFormatLogger<Stream>::log(LogEvent_t logEvent, const cVector3d& vec)
{
	boost::mutex::scoped_lock lock(stream_mutex);
	ostream << lookup(logEvent) << " - " << vec << std::endl;
	return;
}

template <typename Stream>
void HumanFormatLogger<Stream>::log(LogEvent_t logEvent, const Projectile& projectile)
{
	boost::mutex::scoped_lock lock(stream_mutex);
	ostream << lookup(logEvent) << " - " << projectile << std::endl;
	return;
}

template <typename Stream>
void HumanFormatLogger<Stream>::log(LogEvent_t logEvent, const UserPreferences& preferences)
{
	boost::mutex::scoped_lock lock(stream_mutex);
	ostream << lookup(logEvent) << " - " << preferences << std::endl;
	return;
}

#endif

