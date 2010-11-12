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

void MFCUserInterface::switchCamera(cCamera* camera)
{
	m_pMainDlg->camera(camera);
	return;
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

void MFCUserInterface::displayConnectionStateChanged(ConnectionState_t state, Player_t player)
{
	m_pMainDlg->displayConnectionStateChanged(state, player);
	return;
}

void MFCUserInterface::displayGameStateChanged(GameState_t state, Player_t player)
{
	m_pMainDlg->displayGameStateChanged(state, player);
	return;
}

void MFCUserInterface::displayConnectionFailed()
{
	m_pMainDlg->displayConnectionFailed();
	return;
}

void MFCUserInterface::displayFailedToListen()
{
	m_pMainDlg->displayFailedToListen();
	return;
}

void MFCUserInterface::displayNetworkError()
{
	m_pMainDlg->displayNetworkError();
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

void MFCUserInterface::displayGameOver(Player_t player)
{
	m_pMainDlg->displayGameOver(player);
	return;
}

void MFCUserInterface::paint()
{
	m_pMainDlg->paint();
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

void MFCUserInterface::muteVolume(const bool soundOn)
{
	notify(MUTE_VOLUME, &soundOn);
	return;
}

void MFCUserInterface::changeVolume(const int vol)
{
	notify(CHANGE_VOLUME, &vol);
	return;
}

void MFCUserInterface::setMutePref(const bool mute)
{
	m_pMainDlg->setMutePref(mute);
	return;
}

void MFCUserInterface::setVolPref(const int newVolume)
{
	m_pMainDlg->setVolPref(newVolume);
	return;
}
