#ifndef LOG_EVENT_CATEGORY_H
#define LOG_EVENT_CATEGORY_H

#include "stdafx.h"

#include "LogEventEnum.h"

/**
 * Namespace that holds the log event category enum.
 * This protects from naming conflicts.
 */
namespace LogEventCategory
{
	/**
	 * @ingroup Logger
	 * @b package
	 * Enum that contains all the different categories of log events.
	 */
	enum LogEventCategoryEnum
	{
		NETWORK, /**< Events originating from the network. */
		UI, /**< Events originating from the user interface. */
		FALCON, /**< Events originating from the falcon. */
		ZCAM, /**< Events originating from the zcamera. */
	};

	/**
	 * Convert the enum to a string.
	 * @param[in] category The enum to convert to a string.
	 * @return The converted enum.
	 */
	std::string lookup(LogEventCategoryEnum category);

	/**
	 * Determine which category a log event belongs to.
	 * @param[in] logEvent The log event.
	 * @return The category that the event belongs to.
	 */
	LogEventCategoryEnum lookupCategory(LogEvent_t logEvent);
}

typedef LogEventCategory::LogEventCategoryEnum LogEventCategory_t;

#endif
