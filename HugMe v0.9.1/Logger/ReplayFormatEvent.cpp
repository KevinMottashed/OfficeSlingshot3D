#include "ReplayFormatEvent.h"

ReplayFormatEvent::ReplayFormatEvent()
{
}

ReplayFormatEvent::ReplayFormatEvent(long time, LogEvent logEvent) :
	time(time),
	logEvent(logEvent)
{
}

ReplayFormatEvent::~ReplayFormatEvent()
{
}