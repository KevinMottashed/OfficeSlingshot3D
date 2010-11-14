#ifndef REPLAY_FORMAT_EVENT_H
#define REPLAY_FORMAT_EVENT_H

#include "stdafx.h"
#include "boost.h"
#include "LogEventEnum.h"

/**
 * @ingroup Logger
 * @b package
 * The replay format event class is used to represent a replay event.
 * A replay event contains a time at which the event occured where the
 * time is the elapsed time since the application started. This class
 * also contains an enum representing the event that occured.
 */
class ReplayFormatEvent
{
public:
	ReplayFormatEvent(); /**< Constructor. */
	
	/**
	 * Constructor.
	 * @param[in] time The time at which the event occured.
	 * @param[in] logEvent The event that occured.
	 */
	ReplayFormatEvent(long time, LogEvent_t logEvent);
	
	~ReplayFormatEvent(); /**< Destructor. */
	
	long time; /**< The time at which the event occured. */
	LogEvent_t logEvent; /**< The event that happened. */
	
private:
	// the boost serialization library requires access to the serialize function
	friend class boost::serialization::access;
	
	/**
	 * Serialization function that the boost library will use for both
	 * serializaing and deserializing this data type.
	 * @param[in,out] ar The archive where the serialized data will be saved or read from.
	 * @param[in] version The version of the archive.
	 */
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version);
};

//---------------------------------------------
// Template Implementation
//---------------------------------------------

template<class Archive>
void ReplayFormatEvent::serialize(Archive & ar, const unsigned int version)
{
	ar & time;
	ar & logEvent;
	return;
}

#endif