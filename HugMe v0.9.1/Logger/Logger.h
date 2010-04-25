#ifndef LOGGER_H
#define LOGGER_H

#include "NetworkProxy.h"
#include "FalconProxy.h"
#include "GameProxy.h"
#include "UserInterfaceProxy.h"
#include "ZCameraProxy.h"

// abstract class used for logging
class Logger :	public NetworkObserver,
				public FalconObserver,
				public GameObserver,
				public UserInterfaceObserver,
				public ZCameraObserver
{
public:
	Logger();
	virtual ~Logger();

	// updates from observer patterns
	virtual void update(NetworkUpdateContext context, const void* data);
	virtual void update(UserInterfaceUpdateContext context, const void* data);
	virtual void update(FalconUpdateContext context, const void* data);
	virtual void update(GameUpdateContext context, const void* data);	
	virtual void update(ZCameraUpdateContext context, const void* data);

protected:
	// log network events
	virtual void logPeerConnected() = 0;
	virtual void logPeerDisconnected() = 0;
	virtual void logPeerStartGame() = 0;
	virtual void logPeerPauseGame() = 0;
	virtual void logPeerExitGame() = 0;
	virtual void logNetworkError(rc_network error) = 0;
	virtual void logUserNameReceived(const std::string& name) = 0;
	virtual void logChatMessageReceived(const std::string& message) = 0;
	virtual void logVideoDataReceived(const VideoData& data) = 0;
	virtual void logSlingshotPositionReceived(const cVector3d& position) = 0;
	virtual void logProjectileReceived(const Projectile& projectile) = 0;
	virtual void logPullbackReceived() = 0;
	virtual void logReleaseReceived() = 0;
	virtual void logPlayerPositionReceived(const cVector3d& position) = 0;

	// log user interface events
	virtual void logConnect() = 0;
	virtual void logListen() = 0;
	virtual void logDisconnect() = 0;
	virtual void logChangePreferences(const UserPreferences& preferences) = 0;
	virtual void logLocalStartGame() = 0;
	virtual void logLocalPauseGame() = 0;
	virtual void logLocalExitGame() = 0;
	virtual void logCloseApplication() = 0;
	virtual void logSendChatMessage(const std::string& message) = 0;

	// log falcon events
	virtual void logLocalSlingshotPosition(const cVector3d& position) = 0;

	// log zcam events
	virtual void logLocalVideoData(const VideoData& data) = 0;
};

#endif

