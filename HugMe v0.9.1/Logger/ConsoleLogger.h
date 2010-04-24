#ifndef CONSOLE_LOGGER_H_
#define CONSOLE_LOGGER_H_

#include "Logger.h"
#include "ConsoleStream.h"

// concrete logger class
// this logger outputs the log in a human readable format to the visual studio output window
class ConsoleLogger : public Logger
{
public:
	ConsoleLogger();
	virtual ~ConsoleLogger();

protected:
	// log network events
	virtual void logPeerConnected();
	virtual void logPeerDisconnected();
	virtual void logPeerStartGame();
	virtual void logPeerPauseGame();
	virtual void logPeerExitGame();
	virtual void logNetworkError(rc_network error);
	virtual void logUserNameReceived(const std::string& name);
	virtual void logChatMessageReceived(const std::string& message);
	virtual void logVideoDataReceived(const VideoData& data);
	virtual void logSlingshotPositionReceived(const cVector3d& position);
	virtual void logProjectileReceived(const Projectile& projectile);
	virtual void logPullbackReceived();
	virtual void logReleaseReceived();
	virtual void logPlayerPositionReceived(const cVector3d& position);

	// log user interface events
	virtual void logConnect();
	virtual void logListen();
	virtual void logDisconnect();
	virtual void logChangePreferences(const UserPreferences& preferences);
	virtual void logLocalStartGame();
	virtual void logLocalPauseGame();
	virtual void logLocalExitGame();
	virtual void logCloseApplication();
	virtual void logSendChatMessage(const std::string& message);

	// log falcon events
	virtual void logLocalSlingshotPosition(const cVector3d& position);

	// log zcam events
	virtual void logLocalVideoData(const VideoData& data);

private:
	ConsoleLogger(const ConsoleLogger& c); // intentionally not implemented
	ConsoleLogger& operator=(const ConsoleLogger& c); // intentionally not implemented
};

#endif