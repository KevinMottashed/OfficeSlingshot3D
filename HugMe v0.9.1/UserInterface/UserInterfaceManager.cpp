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
	Mediator::instance()->updateLocalUserName(localName);
	return Mediator::instance()->netConnect(ipAddress);
}

rc_network UserInterfaceManager::networkDisconnectButtonPushed()
{
	return Mediator::instance()->netDisconnect();
}

rc_network UserInterfaceManager::networkListenButtonPushed(const string& localName)
{
	Mediator::instance()->updateLocalUserName(localName);
	return Mediator::instance()->netStartListening();
}

void UserInterfaceManager::startGameButtonPushed()
{
	Mediator::instance()->localStartGame();
}

void UserInterfaceManager::exitGameButtonPushed()
{
	Mediator::instance()->localExitGame();
}

void UserInterfaceManager::pauseGameButtonPushed()
{
	Mediator::instance()->localPauseGame();
}

void UserInterfaceManager::closeApplication()
{
	Mediator::instance()->closeApplication();
}

CDialog* UserInterfaceManager::getMainWindow()
{
	return m_pMainDlg;
}

void UserInterfaceManager::notifyNetworkConnectionEstablished()
{
	string remoteUserName = Mediator::instance()->getRemoteUserName();
	getMainWindow()->SendMessage(WM_ON_CONNECT, (WPARAM)&remoteUserName);
}

void UserInterfaceManager::notifyPeerDisconnected()
{
	string remoteUserName = Mediator::instance()->getRemoteUserName();
	getMainWindow()->SendMessage(WM_ON_DISCONNECT, (WPARAM)&remoteUserName);
}

void UserInterfaceManager::notifyNetworkError(rc_network error)
{
	getMainWindow()->SendMessage(WM_ON_NETWORK_ERROR, (WPARAM)&error);
}

void UserInterfaceManager::notifyGameStarted()
{
	string remoteUserName = Mediator::instance()->getRemoteUserName();
	getMainWindow()->SendMessage(WM_ON_START_GAME, (WPARAM)&remoteUserName);
}

void UserInterfaceManager::notifyGamePaused()
{
	string remoteUserName = Mediator::instance()->getRemoteUserName();
	getMainWindow()->SendMessage(WM_ON_PAUSE_GAME, (WPARAM)&remoteUserName);
}

void UserInterfaceManager::notifyGameExited()
{
	string remoteUserName = Mediator::instance()->getRemoteUserName();
	getMainWindow()->SendMessage(WM_ON_EXIT_GAME, (WPARAM)&remoteUserName);
}

rc_network UserInterfaceManager::sendChatButtonPushed(const string& message)
{
	return Mediator::instance()->sendChatMessage(message);
}

void UserInterfaceManager::notifyNewChatMessage(const string& message)
{
	string remoteUserName = Mediator::instance()->getRemoteUserName();
	getMainWindow()->SendMessage(WM_ON_NEW_CHAT_MESSAGE, (WPARAM)&remoteUserName, (LPARAM)&message);
}

void UserInterfaceManager::notifyDisplayNewLocalFrame(VideoData video)
{
	getMainWindow()->SendMessage(WM_ON_NEW_LOCAL_FRAME, (WPARAM)video.rgb, (LPARAM)&video.size);
}

void UserInterfaceManager::notifyDisplayNewRemoteFrame(VideoData video)
{
	getMainWindow()->SendMessage(WM_ON_NEW_REMOTE_FRAME, (WPARAM)video.rgb, (LPARAM)&video.size);
}

void UserInterfaceManager::changeArmBandPort(int armBandPort)
{
	Mediator::instance()->changeArmBandPort(armBandPort);
}

void UserInterfaceManager::changeJacketPort(int jacketPort)
{
	Mediator::instance()->changeJacketPort(jacketPort);
}
