/**
 * @file
 * Convinience header for including all public members of the logger module.
 */

// public interface for the logger module

// The following loggers are provided:
// HumanFormatFileLogger - output log in a human readable format to a file
// HumanFormatConsoleLogger - output log in a human readable format to the console
// ReplayFormatFileLogger - output log in replay format to a file
// ReplayFormatConsoleLogger - output log in replay format to the console

/**
 * @defgroup Logger Logger
 * The logger module is responsible for creating replay files.
 * The logger will save events coming from the network, zcamera, falcon and user interface module.
 * These events are saved in a file so that the replayer can replay those events.
 * The logger can also output in a human readable output.
 */
#include "Logger.h"
#include "HumanFormatLogger.h"
#include "ReplayFormatLogger.h"
#include "LogEventEnum.h"
#include "LogEventCategoryEnum.h"

// forward declarations
class Logger;
template <typename Stream> class HumanFormatLogger;
template <typename Stream> class ReplayFormatLogger;

// typedefs for commonly used template parameters;
typedef HumanFormatLogger<std::ofstream> HumanFormatFileLogger;
typedef HumanFormatLogger<ConsoleStream> HumanFormatConsoleLogger;
typedef ReplayFormatLogger<std::ofstream> ReplayFormatFileLogger;
typedef ReplayFormatLogger<ConsoleStream> ReplayFormatConsoleLogger;

