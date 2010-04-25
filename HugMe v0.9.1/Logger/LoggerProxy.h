// public interface for the logger module

// The following loggers are provided:
// HumanFormatFileLogger - output log in a human readable format to a file
// HumanFormatConsoleLogger - output log in a human readable format to the console

// public members of the logger module
#include "Logger.h"
#include "HumanFormatLogger.h"
//#include "ReplayLogger.h"

// forward declarations
class Logger;
template <typename S, typename T> class HumanFormatLogger;
//class ReplayLogger;

