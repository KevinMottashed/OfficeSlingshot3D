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

UserInterfaceManager::UserInterfaceManager(Controller* pController) :
		m_pController(pController)
{
	m_pMainDlg = new CMainDlg(this);
}

UserInterfaceManager::~UserInterfaceManager()
{
	delete m_pMainDlg;
}

rc_network UserInterfaceManager::networkConnectButtonPushed(const CString& ipAddress, const string& localName)
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

void UserInterfaceManager::notifyNetworkError()
{
	getMainWindow()->SendMessage(WM_ON_NETWORK_ERROR);
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

void UserInterfaceManager::notifyNewChatMessage(const string& message)
{
	// TODO implement
	std::cout << m_pController->getRemoteUserName() << " says " << message << std::endl;
}


