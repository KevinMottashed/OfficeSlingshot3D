// UserInterfaceManager.cpp: implementation of the UserInterfaceManager class.
//
//////////////////////////////////////////////////////////////////////

#include <string>

#include "UserInterfaceManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

using namespace std;

UserInterfaceManager::UserInterfaceManager()
{
	m_pMainDlg = new CMainDlg(this);
}

UserInterfaceManager::~UserInterfaceManager()
{
	delete m_pMainDlg;
}

rc_network UserInterfaceManager::networkConnectButtonPushed(const string& ipAddress, const string& localName)
{
	Controller::instance()->updateLocalUserName(localName);
	return Controller::instance()->netConnect(ipAddress);
}

rc_network UserInterfaceManager::networkDisconnectButtonPushed()
{
	return Controller::instance()->netDisconnect();
}

rc_network UserInterfaceManager::networkListenButtonPushed(const string& localName)
{
	Controller::instance()->updateLocalUserName(localName);
	return Controller::instance()->netStartListening();
}

void UserInterfaceManager::startGameButtonPushed()
{
	Controller::instance()->localStartGame();
}

void UserInterfaceManager::exitGameButtonPushed()
{
	Controller::instance()->localExitGame();
}

void UserInterfaceManager::closeApplication()
{
	Controller::instance()->closeApplication();
}

CDialog* UserInterfaceManager::getMainWindow()
{
	return m_pMainDlg;
}

void UserInterfaceManager::notifyNetworkConnectionEstablished()
{
	string remoteUserName = Controller::instance()->getRemoteUserName();
	getMainWindow()->SendMessage(WM_ON_CONNECT, (WPARAM)&remoteUserName);
}

void UserInterfaceManager::notifyPeerDisconnected()
{
	string remoteUserName = Controller::instance()->getRemoteUserName();
	getMainWindow()->SendMessage(WM_ON_DISCONNECT, (WPARAM)&remoteUserName);
}

void UserInterfaceManager::notifyNetworkError(rc_network error)
{
	getMainWindow()->SendMessage(WM_ON_NETWORK_ERROR, (WPARAM)&error);
}

void UserInterfaceManager::notifyGameStarted()
{
	string remoteUserName = Controller::instance()->getRemoteUserName();
	getMainWindow()->SendMessage(WM_ON_START_GAME, (WPARAM)&remoteUserName);
}

void UserInterfaceManager::notifyGameExited()
{
	string remoteUserName = Controller::instance()->getRemoteUserName();
	getMainWindow()->SendMessage(WM_ON_EXIT_GAME, (WPARAM)&remoteUserName);
}

void UserInterfaceManager::sendChatButtonPushed(const string& message)
{
	Controller::instance()->sendChatMessage(message);
}

void UserInterfaceManager::notifyNewChatMessage(const string& message)
{
	string remoteUserName = Controller::instance()->getRemoteUserName();
	getMainWindow()->SendMessage(WM_ON_NEW_CHAT_MESSAGE, (WPARAM)&remoteUserName, (LPARAM)&message);
}
