// UserInterfaceManager.cpp: implementation of the UserInterfaceManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "chat.h"
#include "UserInterfaceManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

UserInterfaceManager::UserInterfaceManager(Controller* pController) :
		m_pController(pController)
{
	m_pChatDlg = new CChatDlg(this);
}

UserInterfaceManager::~UserInterfaceManager()
{
	delete m_pChatDlg;
}

rc_network UserInterfaceManager::networkConnectButtonPushed(const CString& ipAddress)
{
	return Controller::instance()->netConnect(ipAddress);
}

rc_network UserInterfaceManager::networkDisconnectButtonPushed()
{
	return Controller::instance()->netDisconnect();
}

rc_network UserInterfaceManager::startGameButtonPushed()
{
	return Controller::instance()->startVideo();
}

rc_network UserInterfaceManager::exitGameButtonPushed()
{
	return Controller::instance()->exitVideo();
}

CDialog* UserInterfaceManager::getMainWindow()
{
	return m_pChatDlg;
}

void UserInterfaceManager::notifyNetworkConnectionEstablished()
{
	// TODO implement
	printf("connection established\n");
}

void UserInterfaceManager::notifyPeerDisconnected()
{
	// TODO implement
	printf("peer disconnected\n");
}

void UserInterfaceManager::notifyNetworkError()
{
	// TODO implement
	printf("network error\n");
}

rc_network UserInterfaceManager::networkListenButtonPushed()
{
	return Controller::instance()->netStartListening();
}


