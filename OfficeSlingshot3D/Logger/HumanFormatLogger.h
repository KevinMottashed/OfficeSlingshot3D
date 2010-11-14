#ifndef HUMAN_FORMAT_LOGGER_H
#define HUMAN_FORMAT_LOGGER_H

#include "stdafx.h"
#include "boost.h"
#include "Logger.h"
#include "ConsoleStream.h"

/**
 * @ingroup Logger
 * @b public
 * Concrete logger class that outputs in a human readable format
 */
template <typename Stream>
class HumanFormatLogger : public Logger
{
public:
	/**
	 * Constructor.
	 * @param[in] ostream The stream that the log will be outputed to.
	 */
	HumanFormatLogger(std::auto_ptr<Stream> ostream);

	virtual ~HumanFormatLogger(); /**< Destructor. */

protected:
	/**
	 * Log an event that has no associated data.
	 * @param[in] logEvent The event to be logged.
	 */
	virtual void log(LogEvent_t logEvent);

	/**
	 * Log an event with data of type unsigned int.
	 * @param[in] logEvent The event to be logged.
	 * @param[in] uInt The associated data.
	 */
	virtual void log(LogEvent_t logEvent, unsigned int uInt);

	/**
	 * Log an event with data of type rc_network.
	 * @param[in] logEvent The event to be logged.
	 * @param[in] error The associated data.
	 */
	virtual void log(LogEvent_t logEvent, rc_network error);

	/**
	 * Log an event with data of type string.
	 * @param[in] logEvent The event to be logged.
	 * @param[in] str The associated data.
	 */
	virtual void log(LogEvent_t logEvent, const std::string& str);

	/**
	 * Log an event with data of type cVector3d.
	 * @param[in] logEvent The event to be logged.
	 * @param[in] vec The associated data.
	 */
	virtual void log(LogEvent_t logEvent, const cVector3d& vec);

	/**
	 * Log an event with data of type Projectile.
	 * @param[in] logEvent The event to be logged.
	 * @param[in] projectile The associated data.
	 */
	virtual void log(LogEvent_t logEvent, const Projectile& projectile);

	/**
	 * Log an event with data of type UserPreferences.
	 * @param[in] logEvent The event to be logged.
	 * @param[in] preferences The associated data.
	 */
	virtual void log(LogEvent_t logEvent, const UserPreferences& preferences);

	/**
	 * Log an event with data of type PhysicsSync.
	 * @param[in] logEvent The event to be logged.
	 * @param[in] sync The associated data.
	 */
	virtual void log(LogEvent_t logEvent, const PhysicsSync& sync);

private:
	/**
	 * Copy constructor. Not implemented to protect from use.
	 * @param[in] c The object to copy.
	 */
	HumanFormatLogger(const HumanFormatLogger& c);

	/**
	 * Assignment operator. Not implemented to protect from use.
	 * @param[in] c The object to copy.
	 */
	HumanFormatLogger& operator=(const HumanFormatLogger& c);

	std::auto_ptr<Stream> ostream; /**< The stream where the log will be writen. */
	boost::mutex stream_mutex; /**< Mutex to protect the stream */
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

