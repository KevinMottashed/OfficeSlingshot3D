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

rc_network UserInterfaceManager::networkConnectButtonPushed(const CString& ipAddress, const CString& localName)
{
	Controller::instance()->updateLocalUserName((string)localName);
	return Controller::instance()->netConnect(ipAddress);
}

rc_network UserInterfaceManager::networkDisconnectButtonPushed()
{
	return Controller::instance()->netDisconnect();
}

void UserInterfaceManager::startGameButtonPushed()
{
	Controller::instance()->startGame();
}

void UserInterfaceManager::exitGameButtonPushed()
{
	Controller::instance()->exitGame();
}

void UserInterfaceManager::closeApplication()
{
	m_pController->closeApplication();
}

CDialog* UserInterfaceManager::getMainWindow()
{
	return m_pChatDlg;
}

void UserInterfaceManager::notifyNetworkConnectionEstablished()
{
	string remoteUserName = Controller::instance()->getRemoteUserName();
	getMainWindow()->SendMessage(WM_ON_ACCEPT, (WPARAM)&remoteUserName);
}

void UserInterfaceManager::notifyPeerDisconnected()
{
	string remoteUserName = Controller::instance()->getRemoteUserName();
	getMainWindow()->SendMessage(WM_ON_PEER_DISCONNECT, (WPARAM)&remoteUserName);
}

void UserInterfaceManager::testJacketButtonPressed()
{
	getMainWindow()->MessageBox("Testing Jacket");
	//TODO Implement by Jacket Manager
}

void UserInterfaceManager::testFalconButtonPressed()
{
	getMainWindow()->MessageBox("Testing Falcon");
	//TODO Implement by Jacket Manager
}

void UserInterfaceManager::testCameraButtonPressed()
{
	getMainWindow()->MessageBox("Testing Camera");
	//TODO Implement by Jacket Manager
}

void UserInterfaceManager::notifyNetworkError()
{
	getMainWindow()->SendMessage(WM_ON_NETWORK_ERROR);
}

rc_network UserInterfaceManager::networkListenButtonPushed(const CString& localName)
{
	Controller::instance()->updateLocalUserName((string)localName);
	return Controller::instance()->netStartListening();
}

void UserInterfaceManager::notifyNewChatMessage(const std::string& message)
{
	// TODO implement
	printf("chat %s\n", message.c_str());
}


