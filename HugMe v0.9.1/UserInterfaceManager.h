// UserInterfaceManager.h: interface for the UserInterfaceManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_USERINTERFACEMANAGER_H__DC0E386C_0D7D_44F6_B8B3_0BA8C08D1B1A__INCLUDED_)
#define AFX_USERINTERFACEMANAGER_H__DC0E386C_0D7D_44F6_B8B3_0BA8C08D1B1A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Controller.h"
#include "ChatDlg.h"

// Forward declaration (files include each other)
class Controller;
class CChatDlg;

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

	//-----------------------------------------------
	// Notifications coming from the user interface
	//-----------------------------------------------

	rc_network networkConnectButtonPushed(const CString& ipAddress, const CString& localName);
	rc_network networkListenButtonPushed(const CString& localName);
	rc_network networkDisconnectButtonPushed();

	void startGameButtonPushed();
	void exitGameButtonPushed();

	void testJacketButtonPressed();
	void testFalconButtonPressed();
	void testCameraButtonPressed();

	void closeApplication();


	// returns the applications main window
	CDialog* getMainWindow();

private:
	Controller* m_pController;
	CChatDlg* m_pChatDlg;
};

#endif // !defined(AFX_USERINTERFACEMANAGER_H__DC0E386C_0D7D_44F6_B8B3_0BA8C08D1B1A__INCLUDED_)
