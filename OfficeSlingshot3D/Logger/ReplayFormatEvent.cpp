#include "ReplayFormatEvent.h"

ReplayFormatEvent::ReplayFormatEvent()
{
}

ReplayFormatEvent::ReplayFormatEvent(long time, LogEvent_t logEvent) :
	time(time),
	logEvent(logEvent)
{
}

ReplayFormatEvent::~ReplayFormatEvent()
{
}