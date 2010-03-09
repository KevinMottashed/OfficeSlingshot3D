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

	//-----------------------------------------------
	// Notifications coming from the user interface
	//-----------------------------------------------

	rc_network networkConnectButtonPushed(const CString& ipAddress);
	rc_network networkListenButtonPushed();
	rc_network networkDisconnectButtonPushed();
	rc_network startGameButtonPushed();
	rc_network exitGameButtonPushed();

	// returns the applications main window
	CDialog* getMainWindow();

private:
	Controller* m_pController;
	CChatDlg* m_pChatDlg;
};

#endif // !defined(AFX_USERINTERFACEMANAGER_H__DC0E386C_0D7D_44F6_B8B3_0BA8C08D1B1A__INCLUDED_)
