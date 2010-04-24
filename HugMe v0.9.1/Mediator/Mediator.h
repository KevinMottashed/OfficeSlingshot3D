// Mediator.h: interface for the Mediator class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CONTROLLER_H__11C3ACF3_BC70_49EC_BA36_3849E78C8CB6__INCLUDED_)
#define AFX_CONTROLLER_H__11C3ACF3_BC70_49EC_BA36_3849E78C8CB6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

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
// this class is a singleton
class Mediator :	public NetworkObserver,
					public UserInterfaceObserver
{
public:
	// gets the singleton
	static Mediator* instance();
	virtual ~Mediator();

	// updates for observer patterns
	virtual void update(NetworkUpdateContext context, const void* data);
	virtual void update(UserInterfaceUpdateContext context, const void* data);

	//------------------------------------------
	// Game related
	//------------------------------------------

	// notifies the Mediator that the slingshot position has changed
	void notifyNewLocalSlingshotPosition(const cVector3d& position);

	// notifies the Mediator that a new projectile has been launched
	void notifyNewLocalProjectile(const Projectile& projectile);

	// notifies the Mediator that the position of a player has changed
	void notifyNewLocalPlayerPosition(const cVector3d& position);

	// notifies the Mediator that a slingshot is being pulled back
	void notifyLocalSlingshotPullback();

	// notifies the Mediator that a slingshot has being released
	void notifyLocalSlingshotRelease();	

	// --------------------------------
	// Video related functions
	// --------------------------------

	// notifies the Mediator that new video data has arrived
	void notifyNewLocalVideoData(VideoData video);

	// ------------------------------------
	// User interface related functions
	// ------------------------------------

	// get the main window of the application
	CDialog* getMainWindow();	

private:
	Mediator(); // private for singleton pattern
	Mediator(const Mediator& c); // intentionally not implemented
	Mediator& operator=(const Mediator& c); // intentionally not implemented

	// the singleton
	static Mediator* globalInstance;

	// the network manager, manages everything network related
	NetworkManager* m_pNetworkManager;

	// manages everything GUI related
	UserInterfaceManager* m_pUserInterfaceManager;

	// manages everything falcon pen related
	FalconPenManager* m_pFalconPenManager;

	// manages everything z camera related
	ZCameraManager* m_pZCameraManager;

	// manages the smart clothing
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

	// start the game
	void startGame();

	// pause the game
	void pauseGame();

	// exit the game
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

};

#endif // !defined(AFX_CONTROLLER_H__11C3ACF3_BC70_49EC_BA36_3849E78C8CB6__INCLUDED_)
