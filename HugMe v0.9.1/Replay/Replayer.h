#ifndef REPLAYER_H
#define REPLAYER_H

#include "stdafx.h"
#include "boost.h"
#include "NetworkProxy.h"
#include "UserInterfaceProxy.h"
#include "FalconProxy.h"
#include "ZCameraProxy.h"
#include "LoggerProxy.h"
#include "UserPreferences.h"

class Replayer : public Network, public MFCUserInterface, public Falcon, public ZCamera
{
public:
	Replayer(std::string fileName, const UserPreferences& preferences);
	virtual ~Replayer();

	void startReplay();
	void endReplay();

	//---------------------------------------------------------------------
	// Network
	//---------------------------------------------------------------------

	// modify the connection status
	virtual rc_network listen(const std::string& userName);	
	virtual rc_network connect(const std::string& ipAddress, const std::string& userName);
	virtual rc_network disconnect();

	// send a message to modify the game state
	virtual rc_network sendStartGame();
	virtual rc_network sendPauseGame();
	virtual rc_network sendEndGame();

	// send data to the other player
	virtual rc_network sendUserName(const std::string& userName);
	virtual rc_network sendChatMessage(const std::string& message);
	virtual rc_network sendVideoData(VideoData video);
	virtual rc_network sendPlayerPosition(const cVector3d& position);
	virtual rc_network sendSlingshotPosition(const cVector3d& position);
	virtual rc_network sendProjectile(const Projectile& projectile);
	virtual rc_network sendSlingshotPullback();
	virtual rc_network sendSlingshotRelease();

	virtual bool isConnected() const;

	//---------------------------------------------------------------------
	// Falcon
	//---------------------------------------------------------------------

	// start/stop polling the falcon
	virtual void startPolling();
	virtual void stopPolling();

	//---------------------------------------------------------------------
	// Z-Camera
	//---------------------------------------------------------------------

	// start/stop capturing frames from the camera
	virtual void startCapture();
	virtual void stopCapture();

private:

	// start replaying from the file
	void replay();

	// the time at which the replay was started
	boost::posix_time::ptime startTime;
	std::ifstream file;

	// true if we are connected to a peer
	bool connected;

	// holds the replay thread
	std::auto_ptr<boost::thread> replayThread;
};

#endif
