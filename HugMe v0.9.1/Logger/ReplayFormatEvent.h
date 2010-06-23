#ifndef REPLAY_FORMAT_EVENT_H
#define REPLAY_FORMAT_EVENT_H

#include "stdafx.h"
#include "boost.h"
#include "LogEvent.h"

// The replay format event class is used to represent a replay event
// A replay event contains a time at which the event occured where the 
// time is the elapsed time since the application started. This class
// also contains an enum representing the event that occured.
class ReplayFormatEvent
{
public:
	ReplayFormatEvent();
	ReplayFormatEvent(long time, LogEvent logEvent);
	~ReplayFormatEvent();

	long time;
	LogEvent logEvent;

private:
	// the boost serialization library requires access to the serialize function
	friend class boost::serialization::access;

	// serialization function that the boost library will use for both
	// serializaing and deserializing this data type
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