// public interface for the logger module

// The following loggers are provided:
// HumanFormatFileLogger - output log in a human readable format to a file
// HumanFormatConsoleLogger - output log in a human readable format to the console
// ReplayFormatFileLogger - output log in replay format to a file
// ReplayFormatConsoleLogger - output log in replay format to the console

// public members of the logger module
#include "Logger.h"
#include "HumanFormatLogger.h"
#include "ReplayFormatLogger.h"

// forward declarations
class Logger;
template <typename Stream> class HumanFormatLogger;
template <typename Stream> class ReplayFormatLogger;

// typedefs for commonly used template parameters;
typedef HumanFormatLogger<std::ofstream> HumanFormatFileLogger;
typedef HumanFormatLogger<ConsoleStream> HumanFormatConsoleLogger;
typedef ReplayFormatLogger<std::ofstream> ReplayFormatFileLogger;
typedef ReplayFormatLogger<ConsoleStream> ReplayFormatConsoleLogger;

