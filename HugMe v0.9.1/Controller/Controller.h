// Controller.h: interface for the Controller class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CONTROLLER_H__11C3ACF3_BC70_49EC_BA36_3849E78C8CB6__INCLUDED_)
#define AFX_CONTROLLER_H__11C3ACF3_BC70_49EC_BA36_3849E78C8CB6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NetworkProxy.h"
#include "UserInterfaceProxy.h"
#include "FalconProxy.h"
#include "ZCameraProxy.h"
#include "JacketProxy.h"
#include "GameProxy.h"

// The controller class for the program
// this class is a singleton
class Controller : public NetworkObserver
{
public:
	// gets the singleton
	static Controller* instance();

	virtual ~Controller();

	// ----------------------------
	// Network related functions
	// ----------------------------

	// the local user wishes to start listening for connections
	rc_network netStartListening();

	// the local user wishes to connect to a host
	// this function will attempt to connect us to the proper IP address
	rc_network netConnect(const std::string& ipAddress);

	// the local user wishes to disconnect from the network
	// this function will disconnect us from the network
	rc_network netDisconnect();

	// handles updates received from the network module
	// context is the update subject (user name, connect, disconnect, etc ...)
	// data is the data that accompagnies the update (player position, projectile, etc ...)
	virtual void networkUpdate(NetworkUpdateContext context, void* data);

	//------------------------------------------
	// Game related
	//------------------------------------------

	// we are starting/exiting/pausing the game
	void localStartGame();
	void localExitGame();
	void localPauseGame();

	// notifies the controller that the slingshot position has changed
	void notifyNewLocalSlingshotPosition(const cVector3d& position);

	// notifies the controller that a new projectile has been launched
	void notifyNewLocalProjectile(const Projectile& projectile);

	// notifies the controller that the position of a player has changed
	void notifyNewLocalPlayerPosition(const cVector3d& position);

	// notifies the controller that a slingshot is being pulled back
	void notifyLocalSlingshotPullback();

	// notifies the controller that a slingshot has being released
	void notifyLocalSlingshotRelease();	

	// --------------------------------
	// Video related functions
	// --------------------------------

	// notifies the controller that new video data has arrived
	void notifyNewLocalVideoData(VideoData video);

	// --------------------------------
	// Smart Clothing related functions
	// --------------------------------

	// changes the armband and jacket port numbers
	void changeArmBandPort(int armBandPort);
	void changeJacketPort(int jacketPort);

	// --------------------------------
	// Player info related functions
	// --------------------------------

	std::string getRemoteUserName();

	// update the local player's user name
	void updateLocalUserName(const std::string& name);

	std::string getLocalUserName();

	// ------------------------------------
	// User interface related functions
	// ------------------------------------

	// the local user wishes to send a chat message
	rc_network sendChatMessage(const std::string& message);

	// get the main window of the application
	CDialog* getMainWindow();

	// ------------------------------------
	// System related functions
	// ------------------------------------

	// close the application in a safe way
	void closeApplication();

private:
	Controller(); // private for singleton pattern
	Controller(const Controller& c); // intentionally not implemented
	Controller& operator=(const Controller& c); // intentionally not implemented

	// the singleton
	static Controller* globalInstance;

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

	bool m_bGameIsRunning;

	// It should be noted that the user names are not thread safe.
	// For now this is OK because the UI and network will never try to access to access them at the same time.
	// This is because the UI won't try to access them until the network connection is established.
	// If we ever add a feature where the user names can changes after the connection has been established, then
	// we will need to make these thread safe
	std::string localUserName;
	std::string remoteUserName;

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

};

#endif // !defined(AFX_CONTROLLER_H__11C3ACF3_BC70_49EC_BA36_3849E78C8CB6__INCLUDED_)
