#ifndef MEDIATOR_H
#define MEDIATOR_H

#include "stdafx.h"
#include "NetworkProxy.h"
#include "UserInterfaceProxy.h"
#include "FalconProxy.h"
#include "ZCameraProxy.h"
#include "JacketProxy.h"
#include "GameProxy.h"
#include "LoggerProxy.h"
#include "VideoData.h"
#include "Configuration.h"
#include "SyncLocker.h"

// The Mediator class for the program
class Mediator :	public NetworkObserver,
					public UserInterfaceObserver,
					public ZCameraObserver,
					public FalconObserver,
					public GameObserver
{
public:
	Mediator();
	virtual ~Mediator();

	// updates for observer patterns
	virtual void update(NetworkUpdateContext context, const void* data);
	virtual void update(UserInterfaceUpdateContext context, const void* data);
	virtual void update(ZCameraUpdateContext context, const void* data);
	virtual void update(FalconUpdateContext context, const void* data);
	virtual void update(GameUpdateContext context, const void* data);

	// get the main window of the application
	CDialog* getMainWindow();	

private:
	Mediator(const Mediator& c); // intentionally not implemented
	Mediator& operator=(const Mediator& c); // intentionally not implemented

	// managers for the different modules
	NetworkManager* m_pNetworkManager;
	UserInterfaceManager* m_pUserInterfaceManager;
	FalconPenManager* m_pFalconPenManager;
	ZCameraManager* m_pZCameraManager;
	SmartClothingManager* m_pSmartClothingManager;

	// the game
	Game* m_pGame;

	// the logger, this will eventually be moved out of this class
	// but its too much work to do in one shot
	// leave it here until we have some sort of application initializer
	Logger* m_pLogger;

	// move this out once we have an application initializer
	Configuration* m_pConfiguration;
	mutable CRITICAL_SECTION m_csConfiguration;

	bool m_bGameIsRunning;

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
	void handleRemoteVideoData(VideoData video);
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
	void handleLocalVideoData(VideoData video);

	//--------------------------------------------
	// Falcon Related updates
	//--------------------------------------------

	// slingshot related
	void handleLocalSlingshotPosition(const cVector3d& position);

};

#endif

