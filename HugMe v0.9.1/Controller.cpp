// Controller.cpp: implementation of the Controller class.
//
//////////////////////////////////////////////////////////////////////

#include "Controller.h"

Controller* Controller::globalInstance = NULL;

Controller* Controller::instance()
{
	if (globalInstance == NULL)
	{
		globalInstance = new Controller();
	}
	return globalInstance;
}

rc_network Controller::netStartListening()
{
	return m_pNetworkManager->startListening();
}

void Controller::notifyNetworkConnectionAccepted()
{
	// send the connection accepted message to the user interface process
	m_pUserInterfaceManager->notifyNetworkConnectionEstablished();
}

void Controller::notifyPeerDisconnected()
{
	// notify the user interface that the network connection as been disconnected
	m_pUserInterfaceManager->notifyPeerDisconnected();
}

void Controller::notifyNetworkError()
{
	// notify the user interface that the network connection as been disconnected
	m_pUserInterfaceManager->notifyNetworkError();
}

rc_network Controller::netConnect(const CString& ipAddress)
{
	return m_pNetworkManager->connect(ipAddress);
}

rc_network Controller::netDisconnect()
{
	return m_pNetworkManager->disconnect();
}

void Controller::startGame()
{
	if (m_bGameIsRunning)
	{
		return; // already started
	}
	m_bGameIsRunning = true;
	m_pGame->start();
	m_pFalconPenManager->start();
	m_pZCameraManager->start();
}

void Controller::exitGame()
{
	m_bGameIsRunning = false;
	m_pGame->stop();
	m_pFalconPenManager->stop();
	m_pZCameraManager->stop();
}

void Controller::closeApplication()
{
	// the only thing to do is to close the sockets so the other end has a clean disconnect
	m_pNetworkManager->disconnect();
}

Controller::Controller() :
		m_bGameIsRunning(false)
{
	m_pUserInterfaceManager = new UserInterfaceManager(this);
	m_pNetworkManager = new NetworkManager(this);
	m_pFalconPenManager = new FalconPenManager(this);
	m_pZCameraManager = new ZCameraManager(this);
	m_pSmartClothingManager = new SmartClothingManager(this);
	m_pGame = new Game(this);
}

Controller::~Controller()
{
	delete m_pNetworkManager;
	delete m_pUserInterfaceManager;
	delete m_pFalconPenManager;
	delete m_pZCameraManager;
	delete m_pSmartClothingManager;
	delete m_pGame;
}

void Controller::updateRemoteUserName(const std::string& name)
{
	remoteUserName = name;
	return;
}

std::string Controller::getRemoteUserName()
{
	return remoteUserName;
}

void Controller::updateLocalUserName(const std::string& name)
{
	localUserName = name;
	return;
}

std::string Controller::getLocalUserName()
{
	return localUserName;
}

CDialog* Controller::getMainWindow()
{
	return m_pUserInterfaceManager->getMainWindow();
}

void Controller::notifyNewLocalVideoData(const std::vector<BYTE>& vRGB)
{
	// TODO implement
	printf("new local video data has arrived\n");
}

void Controller::notifyNewRemoteVideoData(const std::vector<BYTE>& vRGB)
{
	// TODO implement
	printf("new remote video data has arrived\n");
}

void Controller::notifyNewChatMessage(const std::string& message)
{
	m_pUserInterfaceManager->notifyNewChatMessage(message);
}

void Controller::notifyNewLocalSlingshotPosition(const cVector3d& position)
{
	// TODO send it over the network too
	m_pGame->setLocalSlingshotPosition(position);
}

void Controller::notifyNewRemoteSlingshotPosition(const cVector3d& position)
{
	m_pGame->setRemoteSlingshotPosition(position);
}

void Controller::notifyNewLocalPlayerPosition(const cVector3d& position)
{
	// TODO send it over the network too
	m_pGame->setLocalPlayerPosition(position);
}

void Controller::notifyNewRemotePlayerPosition(const cVector3d& position)
{
	m_pGame->setRemotePlayerPosition(position);
}

