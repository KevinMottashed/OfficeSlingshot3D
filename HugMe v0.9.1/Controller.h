// Controller.h: interface for the Controller class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CONTROLLER_H__11C3ACF3_BC70_49EC_BA36_3849E78C8CB6__INCLUDED_)
#define AFX_CONTROLLER_H__11C3ACF3_BC70_49EC_BA36_3849E78C8CB6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NetworkManager.h"
#include "NetworkCodes.h"
#include "NetworkSignals.h"
#include "UserInterfaceManager.h"
#include "FalconPenManager.h"
#include "ZCameraManager.h"
#include "SmartClothingManager.h"
#include "Game.h"

// Forward declarations (files include each other)
class NetworkManager;
class UserInterfaceManager;
class FalconPenManager;
class ZCameraManager;
class SmartClothingManager;
class Game;

// The controller class for the program
// this class is a singleton
class Controller
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

	// notifies the controller that the network connection has been accepted
	void notifyNetworkConnectionAccepted();

	// notifies the controller that the peer has disconnected
	void notifyPeerDisconnected();

	// notifies the controller that there has been a network error
	void notifyNetworkError(rc_network error);

	//------------------------------------------
	// Game related
	//------------------------------------------

	// we are starting/exiting the game
	void localStartGame();
	void localExitGame();

	// notifies the controller that the peer has started or exited the game
	void notifyPeerStartGame();
	void notifyPeerExitGame();

	// notifies the controller that the slingshot position has changed
	void notifyNewLocalSlingshotPosition(const cVector3d& position);
	void notifyNewRemoteSlingshotPosition(const cVector3d& position);

	// notifies the controller that a new projectile has been launched
	void notifyNewLocalProjectile(const cVector3d& position, const cVector3d& speed);
	void notifyNewRemoteProjectile(const cVector3d& position, const cVector3d& speed);

	// notifies the controller that the position of a player has changed
	void notifyNewLocalPlayerPosition(const cVector3d& position);
	void notifyNewRemotePlayerPosition(const cVector3d& position);

	// --------------------------------
	// Video related functions
	// --------------------------------

	// notifies the controller that new video data has arrived
	void notifyNewLocalVideoData(const std::vector<BYTE>& vRGB);
	void notifyNewRemoteVideoData(const std::vector<BYTE>& vRGB);

	// --------------------------------
	// Player info related functions
	// --------------------------------

	// update the remote player's user name
	void updateRemoteUserName(const std::string& name);

	std::string getRemoteUserName();

	// update the local player's user name
	void updateLocalUserName(const std::string& name);

	std::string getLocalUserName();

	// ------------------------------------
	// User interface related functions
	// ------------------------------------

	// the local user wishes to send a chat message
	void sendChatMessage(const std::string& message);

	// the peer has sent us a chat message
	void notifyNewChatMessage(const std::string& message);

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

	// exit the game
	void exitGame();

};

#endif // !defined(AFX_CONTROLLER_H__11C3ACF3_BC70_49EC_BA36_3849E78C8CB6__INCLUDED_)
