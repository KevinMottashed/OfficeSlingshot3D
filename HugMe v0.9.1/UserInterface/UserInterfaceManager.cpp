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

UserInterfaceManager::UserInterfaceManager(const UserPreferences& preferences)
{
	m_pMainDlg = new CMainDlg(this, preferences);
}

UserInterfaceManager::~UserInterfaceManager()
{
	delete m_pMainDlg;
}

void UserInterfaceManager::networkConnectButtonPushed()
{
	notify(CONNECT);	
	return;
}

void UserInterfaceManager::networkDisconnectButtonPushed()
{
	notify(DISCONNECT);
	return;
}

void UserInterfaceManager::networkListenButtonPushed()
{
	notify(LISTEN);
	return;
}

void UserInterfaceManager::startGameButtonPushed()
{
	notify(START_GAME);
	return;
}

void UserInterfaceManager::exitGameButtonPushed()
{
	notify(EXIT_GAME);
	return;
}

void UserInterfaceManager::pauseGameButtonPushed()
{
	notify(PAUSE_GAME);
	return;
}

void UserInterfaceManager::closeApplication()
{
	notify(EXIT_APPLICATION);
	return;
}

CDialog* UserInterfaceManager::getMainWindow()
{
	return m_pMainDlg;
}

void UserInterfaceManager::displayConnectionEstablished()
{
	m_pMainDlg->displayConnectionEstablished();
	return;
}

void UserInterfaceManager::displayConnectionFailed()
{
	m_pMainDlg->displayConnectionFailed();
	return;
}

void UserInterfaceManager::displayListening()
{
	m_pMainDlg->displayListening();
	return;
}

void UserInterfaceManager::displayFailedToListen()
{
	m_pMainDlg->displayFailedToListen();
	return;
}

void UserInterfaceManager::displayPeerDisconnected()
{
	m_pMainDlg->displayPeerDisconnected();
	return;
}

void UserInterfaceManager::displayNetworkError()
{
	m_pMainDlg->displayNetworkError();
	return;
}

void UserInterfaceManager::displayGameStarted()
{
	m_pMainDlg->displayGameStarted();
	return;
}

void UserInterfaceManager::displayGamePaused()
{
	m_pMainDlg->displayGamePaused();
	return;
}

void UserInterfaceManager::displayGameExited()
{
	m_pMainDlg->displayGameExited();
	return;
}

void UserInterfaceManager::sendChatButtonPushed(const string& message)
{
	notify(CHAT_MESSAGE, &message);
	return;
}

void UserInterfaceManager::displayPeerChatMessage(const string& message)
{
	m_pMainDlg->displayPeerChatMessage(message);
	return;
}

void UserInterfaceManager::displayLocalFrame(VideoData video)
{
	m_pMainDlg->displayLocalFrame(video);
	return;	
}

void UserInterfaceManager::displayRemoteFrame(VideoData video)
{
	m_pMainDlg->displayRemoteFrame(video);
	return;
}

void UserInterfaceManager::setPeerUserName(const std::string& name)
{
	m_pMainDlg->setPeerUserName(name);
	return;
}

void UserInterfaceManager::changePreferences(const UserPreferences& preferences)
{
	notify(PREFERENCES, &preferences);
	return;
}

