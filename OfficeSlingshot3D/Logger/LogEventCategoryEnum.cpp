#include "LogEventCategoryEnum.h"

std::string LogEventCategory::lookup(LogEventCategoryEnum category)
{
	switch (category)
	{
		case NETWORK:
		{
			return "network";
		}
		case UI:
		{
			return "UI";
		}
		case FALCON:
		{
			return "falcon";
		}
		case ZCAM:
		{
			return "zcam";
		}
	}
	return "unknown";
}

LogEventCategory_t LogEventCategory::lookupCategory(LogEvent_t logEvent)
{
	if (logEvent >= LogEvent::START_NETWORK && logEvent < LogEvent::END_NETWORK)
	{
		return NETWORK;
	}
	else if (logEvent >= LogEvent::START_UI && logEvent < LogEvent::END_UI)
	{
		return UI;
	}
	else if (logEvent >= LogEvent::START_FALCON && logEvent < LogEvent::END_FALCON)
	{
		return FALCON;
	}
	else if (logEvent >= LogEvent::START_ZCAM && logEvent < LogEvent::END_ZCAM)
	{
		return ZCAM;
	}
	else
	{
		// uh oh, all events need a category
		assert(false);
		return NETWORK;
	}	
}

