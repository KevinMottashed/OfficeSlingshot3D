#ifndef REPLAY_FORMAT_LOGGER_H
#define REPLAY_FORMAT_LOGGER_H

#include "stdafx.h"
#include "boost.h"

#include "Logger.h"
#include "ConsoleStream.h"
#include "Projectile.h"
#include "ReplayFormatEvent.h"

/**
 * @ingroup Logger
 * @b public
 * Concrete logger class that outputs in the replay format
 */
template <typename Stream>
class ReplayFormatLogger : public Logger
{
public:
	/**
	 * Constructor.
	 * @param[in] ostream The stream that the log will be outputed to.
	 */
	ReplayFormatLogger(std::auto_ptr<Stream> ostream);

	virtual ~ReplayFormatLogger(); /**< Destructor. */

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
	ReplayFormatLogger(const ReplayFormatLogger& c);

	/**
	 * Assignment operator. Not implemented to protect from use.
	 * @param[in] c The object to copy.
	 */
	ReplayFormatLogger& operator=(const ReplayFormatLogger& c);

	/**
	 * Get the elapsed time since the application was initialized.
	 * @return The elapsed time in ms.
	 */
	long getElapsedTimeInMs() const;

	/**
	 * This function is a helper for serializing the ReplayFormatEvent type
	 * The serialization library doesn't like to serialize non-const types
	 * so this creates and returns a const ReplayFormatEvent.
	 * @param[in] e The event to convert
	 * @return The converted replay format event.
	 */
	const ReplayFormatEvent toReplayEvent(LogEvent_t e) const;

	std::auto_ptr<Stream> ostream; /**< The stream that the archive uses. */
	const boost::posix_time::ptime start; /**< The time at which the application was initialized. */ 

	boost::archive::text_oarchive archive; /**< The archive to save the events to. */
	boost::mutex archive_mutex; /**< Protects the archive from synchronous usage. */
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
