#ifndef NETWORK_H
#define NETWORK_H

#include "NetworkSubject.h"
#include "stdafx.h"
#include "VideoData.h"
#include "chai3d.h"
#include "NetworkCodes.h"
#include "Projectile.h"

// abstract class representing the networking component of the application
// all network interfaces should be able to send and receive data
class Network : public NetworkSubject
{
public:
	Network();
	virtual ~Network();

	// modify the connection status
	virtual rc_network listen(const std::string& userName) = 0;	
	virtual rc_network connect(const std::string& ipAddress, const std::string& userName) = 0;
	virtual rc_network disconnect() = 0;

	// send a message to modify the game state
	virtual rc_network sendStartGame() = 0;
	virtual rc_network sendPauseGame() = 0;
	virtual rc_network sendEndGame() = 0;

	// send data to the other player
	virtual rc_network sendUserName(const std::string& userName) = 0;
	virtual rc_network sendChatMessage(const std::string& message) = 0;
	virtual rc_network sendVideoData(VideoData video) = 0;
	virtual rc_network sendPlayerPosition(const cVector3d& position) = 0;
	virtual rc_network sendSlingshotPosition(const cVector3d& position) = 0;
	virtual rc_network sendProjectile(const Projectile& projectile) = 0;
	virtual rc_network sendSlingshotPullback() = 0;
	virtual rc_network sendSlingshotRelease() = 0;

	// true if  we are connected to a peer
	virtual bool isConnected() const = 0;
};

#endif