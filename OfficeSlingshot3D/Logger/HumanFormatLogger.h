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
	HumanFormatLogger(std::auto_ptr<Stream> ostream);

	virtual ~HumanFormatLogger();

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
	HumanFormatLogger(const HumanFormatLogger& c); // intentionally not implemented
	HumanFormatLogger& operator=(const HumanFormatLogger& c); // intentionally not implemented

	std::auto_ptr<Stream> ostream;
	boost::mutex stream_mutex;
};

//---------------------------------------------
// Template Implementation
//---------------------------------------------

template <typename Stream>
HumanFormatLogger<Stream>::HumanFormatLogger(std::auto_ptr<Stream> ostream) : 
	ostream(ostream)
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
	*ostream << lookup(logEvent) << std::endl;
	return;
}

template <typename Stream>
void HumanFormatLogger<Stream>::log(LogEvent_t logEvent, unsigned int uInt)
{
	boost::mutex::scoped_lock lock(stream_mutex);
	*ostream << lookup(logEvent) << " - " << uInt << std::endl;
	return;
}

template <typename Stream>
void HumanFormatLogger<Stream>::log(LogEvent_t logEvent, rc_network error)
{
	boost::mutex::scoped_lock lock(stream_mutex);
	*ostream << lookup(logEvent) << " - " << lookup(error) << std::endl;
	return;
}

template <typename Stream>
void HumanFormatLogger<Stream>::log(LogEvent_t logEvent, const std::string& str)
{
	boost::mutex::scoped_lock lock(stream_mutex);
	*ostream << lookup(logEvent) << " - " << str << std::endl;
	return;
}

template <typename Stream>
void HumanFormatLogger<Stream>::log(LogEvent_t logEvent, const cVector3d& vec)
{
	boost::mutex::scoped_lock lock(stream_mutex);
	*ostream << lookup(logEvent) << " - " << vec << std::endl;
	return;
}

template <typename Stream>
void HumanFormatLogger<Stream>::log(LogEvent_t logEvent, const Projectile& projectile)
{
	boost::mutex::scoped_lock lock(stream_mutex);
	*ostream << lookup(logEvent) << " - " << projectile << std::endl;
	return;
}

template <typename Stream>
void HumanFormatLogger<Stream>::log(LogEvent_t logEvent, const UserPreferences& preferences)
{
	boost::mutex::scoped_lock lock(stream_mutex);
	*ostream << lookup(logEvent) << " - " << preferences << std::endl;
	return;
}

template <typename Stream>
void HumanFormatLogger<Stream>::log(LogEvent_t logEvent, const PhysicsSync& sync)
{
	boost::mutex::scoped_lock lock(stream_mutex);
	*ostream << lookup(logEvent) << " - " << sync << std::endl;
	return;
}

#endif

