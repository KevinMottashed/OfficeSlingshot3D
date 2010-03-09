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

	// a network connection has been accepted, notify the user interface
	void notifyNetworkConnectionAccepted();

	// --------------------------------
	// Player info related functions
	// --------------------------------

	// update the remote player's user name
	void updateRemoteUserName(const std::string& name);

	// ------------------------------------
	// User interface related functions
	// ------------------------------------

	CDialog* getMainWindow();

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
	std::string localUserName;
	std::string remoteUserName;
};

#endif // !defined(AFX_CONTROLLER_H__11C3ACF3_BC70_49EC_BA36_3849E78C8CB6__INCLUDED_)
