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

// Forward declaration (files include each other)
class NetworkManager;

// The controller class for the program
// this class is a singleton
class Controller  
{
public:
	// gets the singleton
	static Controller* instance();

	virtual ~Controller();

	// start listening for connections on the network
	rc_network netStartListening();

	// attempt to connect to a host
	rc_network netConnect(CString ipAddress);

	// a network connection has been accepted, notify the user interface
	void notifyNetworkConnectionAccepted();

	void setChatWindow(CWnd* pChatWindow);

private:
	Controller(); // private for singleton pattern
	Controller(const Controller& c); // intentionally not implemented
	Controller& operator=(const Controller& c); // intentionally not implemented

	// the singleton
	static Controller* globalInstance;

	NetworkManager* m_pNetworkManager; // the network manager
	CWnd* m_pChatWindow; // the user interface
};

#endif // !defined(AFX_CONTROLLER_H__11C3ACF3_BC70_49EC_BA36_3849E78C8CB6__INCLUDED_)
