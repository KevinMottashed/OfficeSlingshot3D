// UserInterfaceManager.h: interface for the UserInterfaceManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_USERINTERFACEMANAGER_H__DC0E386C_0D7D_44F6_B8B3_0BA8C08D1B1A__INCLUDED_)
#define AFX_USERINTERFACEMANAGER_H__DC0E386C_0D7D_44F6_B8B3_0BA8C08D1B1A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>

#include "Controller.h"
#include "MainDlg.h"

using namespace std;

// Forward declaration (files include each other)
class Controller;

class UserInterfaceManager  
{
public:
	UserInterfaceManager(Controller* pController);
	virtual ~UserInterfaceManager();

	//----------------------------------------
	// Notifications for the user interface
	//----------------------------------------

	// updates the user interface to reflect an established network connection
	void notifyNetworkConnectionEstablished();

	// updates the user interface to reflect a disconnected network connection
	void notifyPeerDisconnected();

	// updates the user interface to reflect an error in the network connection
	void notifyNetworkError();

	// updates the user interface to reflect a game started
	void notifyGameStarted();

	// updates the user interface to reflect a game exited
	void notifyGameExited();

	// update the user interface to display a chat message
	void notifyNewChatMessage(const std::string& message);

	//-----------------------------------------------
	// Notifications coming from the user interface
	//-----------------------------------------------

	rc_network networkConnectButtonPushed(const CString& ipAddress, const string& localName);
	rc_network networkListenButtonPushed(const string& localName);
	rc_network networkDisconnectButtonPushed();

	void startGameButtonPushed();
	void exitGameButtonPushed();

	void closeApplication();


	// returns the applications main window
	CDialog* getMainWindow();

private:
	Controller* m_pController;
	CDialog* m_pMainDlg;
};

#endif // !defined(AFX_USERINTERFACEMANAGER_H__DC0E386C_0D7D_44F6_B8B3_0BA8C08D1B1A__INCLUDED_)
