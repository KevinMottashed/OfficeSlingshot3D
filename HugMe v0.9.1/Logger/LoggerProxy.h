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
template <typename S, typename T1> class HumanFormatLogger;
template <typename S, typename T1, typename T2> class ReplayFormatLogger;

// typedefs for commonly used template parameters;
typedef HumanFormatLogger<std::ofstream, char*> HumanFormatFileLogger;
typedef HumanFormatLogger<ConsoleStream> HumanFormatConsoleLogger;
typedef ReplayFormatLogger<std::ofstream, char*, unsigned int> ReplayFormatFileLogger;
typedef ReplayFormatLogger<ConsoleStream> ReplayFormatConsoleLogger;

