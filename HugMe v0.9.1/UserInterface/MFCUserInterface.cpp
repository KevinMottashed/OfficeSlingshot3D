#include "MFCUserInterface.h"

using namespace std;

MFCUserInterface::MFCUserInterface(const UserPreferences& preferences)
{
	m_pMainDlg = new CMainDlg(this, preferences);
}

MFCUserInterface::~MFCUserInterface()
{
	delete m_pMainDlg;
}

void MFCUserInterface::networkConnectButtonPushed()
{
	notify(CONNECT);	
	return;
}

void MFCUserInterface::networkDisconnectButtonPushed()
{
	notify(DISCONNECT);
	return;
}

void MFCUserInterface::networkListenButtonPushed()
{
	notify(LISTEN);
	return;
}

void MFCUserInterface::startGameButtonPushed()
{
	notify(START_GAME);
	return;
}

void MFCUserInterface::exitGameButtonPushed()
{
	notify(EXIT_GAME);
	return;
}

void MFCUserInterface::pauseGameButtonPushed()
{
	notify(PAUSE_GAME);
	return;
}

void MFCUserInterface::closeApplication()
{
	notify(EXIT_APPLICATION);
	return;
}

CDialog* MFCUserInterface::getMainWindow()
{
	return m_pMainDlg;
}

void MFCUserInterface::displayConnectionEstablished()
{
	m_pMainDlg->displayConnectionEstablished();
	return;
}

void MFCUserInterface::displayConnectionFailed()
{
	m_pMainDlg->displayConnectionFailed();
	return;
}

void MFCUserInterface::displayListening()
{
	m_pMainDlg->displayListening();
	return;
}

void MFCUserInterface::displayFailedToListen()
{
	m_pMainDlg->displayFailedToListen();
	return;
}

void MFCUserInterface::displayPeerDisconnected()
{
	m_pMainDlg->displayPeerDisconnected();
	return;
}

void MFCUserInterface::displayNetworkError()
{
	m_pMainDlg->displayNetworkError();
	return;
}

void MFCUserInterface::displayGameStarted()
{
	m_pMainDlg->displayGameStarted();
	return;
}

void MFCUserInterface::displayGamePaused()
{
	m_pMainDlg->displayGamePaused();
	return;
}

void MFCUserInterface::displayGameExited()
{
	m_pMainDlg->displayGameExited();
	return;
}

void MFCUserInterface::sendChatButtonPushed(const string& message)
{
	notify(CHAT_MESSAGE, &message);
	return;
}

void MFCUserInterface::displayPeerChatMessage(const string& message)
{
	m_pMainDlg->displayPeerChatMessage(message);
	return;
}

void MFCUserInterface::displayLocalFrame(VideoData video)
{
	m_pMainDlg->displayLocalFrame(video);
	return;	
}

void MFCUserInterface::displayRemoteFrame(VideoData video)
{
	m_pMainDlg->displayRemoteFrame(video);
	return;
}

void MFCUserInterface::setPeerUserName(const std::string& name)
{
	m_pMainDlg->setPeerUserName(name);
	return;
}

void MFCUserInterface::changePreferences(const UserPreferences& preferences)
{
	notify(PREFERENCES, &preferences);
	return;
}

