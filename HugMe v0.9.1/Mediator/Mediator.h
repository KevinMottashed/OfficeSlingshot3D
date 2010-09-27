#ifndef MEDIATOR_H
#define MEDIATOR_H

#include "stdafx.h" // STL + windows
#include "boost.h" // boost
#include "NetworkProxy.h"
#include "UserInterfaceProxy.h"
#include "FalconProxy.h"
#include "ZCameraProxy.h"
#include "JacketProxy.h"
#include "LoggerProxy.h"
#include "VideoData.h"
#include "Configuration.h"
#include "SyncLocker.h"
#include "AudioProxy.h"

/**
 * This class mediates the interactions between the device and the game.
 * All device modules report events to the mediator. The mediator will then
 * interpret these events and notify the game.
 */
class Mediator :	public NetworkObserver,
					public UserInterfaceObserver,
					public ZCameraObserver,
					public FalconObserver
{
public:
	Mediator(	boost::shared_ptr<Network> network,
				boost::shared_ptr<Falcon> falcon,
				boost::shared_ptr<IZCamera> zcamera,
				boost::shared_ptr<UserInterface> userInterface,
				boost::shared_ptr<Configuration> configuration);
	virtual ~Mediator();

	// updates for observer patterns
	virtual void update(NetworkUpdateContext context, const void* data);
	virtual void update(UserInterfaceUpdateContext context, const void* data);
	virtual void update(ZCameraUpdateContext context, const void* data);
	virtual void update(FalconUpdateContext context, const void* data);

private:
	Mediator(const Mediator& c); // intentionally not implemented
	Mediator& operator=(const Mediator& c); // intentionally not implemented

	// managers for the different modules
	boost::shared_ptr<Network> network;
	boost::shared_ptr<UserInterface> userInterface;
	boost::shared_ptr<Falcon> falcon;
	boost::shared_ptr<IZCamera> zcamera;
	boost::shared_ptr<SmartClothingManager> smartClothing;
	Audio audio;

	// the game
	Game game;

	// the user preferences
	boost::shared_ptr<Configuration> configuration;
	mutable CRITICAL_SECTION configurationMutex;

	// the current state of the game
	GameState_t gameState;

	// alter the game state
	void startGame();
	void pauseGame();
	void exitGame();

	//--------------------------------------------
	// Network Related updates
	//--------------------------------------------

	// connection status
	void handlePeerConnected();
	void handlePeerDisconnected();
	void handleNetworkError(rc_network error);

	// game status
	void handlePeerStartGame();
	void handlePeerPauseGame();
	void handlePeerExitGame();

	// data reception
	void handleUserName(const std::string& name);
	void handleChatMessage(const std::string& message);
	void handleRemoteVideoData(const VideoData& video);
	void handleRemoteSlingshotPosition(const cVector3d& position);
	void handleRemoteProjectile(const Projectile& projectile);
	void handleRemotePlayerPosition(const cVector3d& position);
	void handleRemotePullback();
	void handleRemoteRelease();

	//--------------------------------------------
	// User Interface Related updates
	//--------------------------------------------

	// connection status updates
	// the user wants to change the connection status
	void connect();
	void listen();
	void disconnect();

	// the user changed his preferences
	void changePreferences(const UserPreferences& preferences);

	// the user wants to start/exit/pause the game
	void localStartGame();
	void localExitGame();
	void localPauseGame();

	// user wants to close the application
	void closeApplication();
	void sendChatMessage(const std::string& message);

	//--------------------------------------------
	// ZCamera Related updates
	//--------------------------------------------

	// new local video
	void handleLocalVideoData(VideoData& video);

	//--------------------------------------------
	// Falcon Related updates
	//--------------------------------------------

	// slingshot related
	void handleLocalSlingshotPosition(const cVector3d& position);

	// notify that a new ball was shot
	void notifyNewBallShot(const cVector3d& force);
	void receiveNewBall(const cVector3d& force);

};

#endif

