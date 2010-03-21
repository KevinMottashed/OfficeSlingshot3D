// UserInterfaceManager.cpp: implementation of the UserInterfaceManager class.
//
//////////////////////////////////////////////////////////////////////

#include "UserInterfaceManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

UserInterfaceManager::UserInterfaceManager(Controller* pController) :
		m_pController(pController)
{
	m_pMainDlg = new CMainDlg(this);
}

UserInterfaceManager::~UserInterfaceManager()
{
	delete m_pMainDlg;
}

rc_network UserInterfaceManager::networkConnectButtonPushed(const CString& ipAddress, const CString& localName)
{
	Controller::instance()->updateLocalUserName((std::string)localName);
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
	return m_pMainDlg;
}

void UserInterfaceManager::notifyNetworkConnectionEstablished()
{
	// TODO implement
}

void UserInterfaceManager::notifyPeerDisconnected()
{
	// TODO implement
}

void UserInterfaceManager::notifyNetworkError()
{
	// TODO implement
}

rc_network UserInterfaceManager::networkListenButtonPushed(const CString& localName)
{
	Controller::instance()->updateLocalUserName((std::string)localName);
	return Controller::instance()->netStartListening();
}

void UserInterfaceManager::notifyNewChatMessage(const std::string& message)
{
	// TODO implement
	std::cout << m_pController->getRemoteUserName() << " says " << message << std::endl;
}


