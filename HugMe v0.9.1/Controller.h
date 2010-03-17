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

// Forward declaration (files include each other)
class NetworkManager;
class UserInterfaceManager;

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

	// start listening for connections on the network
	rc_network netStartListening();

	// attempt to connect to a host
	rc_network netConnect(const CString& ipAddress);

	// attempt to disconnect to a host
	rc_network netDisconnect();

	// start the game
	void startGame();

	// exit the game
	void exitGame();

	// a network connection has been accepted, notify the user interface
	void notifyNetworkConnectionAccepted();

	// the network the peer has disconnected, notify the user interface
	void notifyPeerDisconnected();

	// the network has been disconnected in error, notify the user interface
	void notifyNetworkError();

	// new video data
	void notifyNewVideoData(const std::vector<BYTE>& vRGB, const std::vector<BYTE>& vDepth, const std::vector<BYTE>& vAR);

	// new tactile data
	void notifyNewTactileData(const std::vector<BYTE>& vTactile);

	// new chat message
	void notifyNewChatMessage(const std::string& message);

	// --------------------------------
	// Player info related functions
	// --------------------------------

	// update the remote player's user name
	void updateRemoteUserName(const std::string& name);

	string getRemoteUserName();

	// update the local player's user name
	void updateLocalUserName(const std::string& name);

	string getLocalUserName();

	// ------------------------------------
	// User interface related functions
	// ------------------------------------

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

	// It should be noted that the user names are not thread safe.
	// For now this is OK because the UI and network will never try to access to access them at the same time.
	// This is because the UI won't try to access them until the network connection is established.
	// If we ever add a feature where the user names can changes after the connection has been established, then
	// we will need to make these thread safe
	std::string localUserName;
	std::string remoteUserName;
};

#endif // !defined(AFX_CONTROLLER_H__11C3ACF3_BC70_49EC_BA36_3849E78C8CB6__INCLUDED_)
