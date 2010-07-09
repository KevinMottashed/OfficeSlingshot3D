#ifndef LOG_EVENT_CATEGORY_H
#define LOG_EVENT_CATEGORY_H

#include "stdafx.h"

#include "LogEventEnum.h"

// This enum represents the different categories of input events that the system can receive
// These categories are network, user interface, falcon and zcamera
namespace LogEventCategory
{
	enum LogEventCategoryEnum
	{
		NETWORK,
		UI,
		FALCON,
		ZCAM
	};

	// convert the enum to a string
	std::string lookup(LogEventCategoryEnum category);

	// determine which category an input event belongs to
	LogEventCategoryEnum lookupCategory(LogEvent_t logEvent);
}

typedef LogEventCategory::LogEventCategoryEnum LogEventCategory_t;

#endif
