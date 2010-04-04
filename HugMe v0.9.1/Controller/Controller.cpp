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

// when the network is connected we want to notify the UI
void Controller::notifyNetworkConnectionAccepted()
{
	// send the connection accepted message to the user interface process
	m_pUserInterfaceManager->notifyNetworkConnectionEstablished();
}

// when the peer disconnects we want to exit the game and notify the UI
void Controller::notifyPeerDisconnected()
{
	// exit the game when the peer disconnects
	exitGame();

	// notify the user interface that the network connection has been disconnected
	m_pUserInterfaceManager->notifyPeerDisconnected();
}

// when there has been a network error we should exit the game and notify the UI
void Controller::notifyNetworkError(rc_network error)
{
	// exit the game when a network error occurs
	exitGame();

	// notify the user interface that the network connection as been disconnected
	m_pUserInterfaceManager->notifyNetworkError(error);
}

rc_network Controller::netConnect(const std::string& ipAddress)
{
	return m_pNetworkManager->connect(ipAddress);
}

// we want to exit the game and disconnect when the local user wishes to disconnect
rc_network Controller::netDisconnect()
{
	// exit the game when we disconnect
	exitGame();

	// tell the network manager to disconnect us
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
	// by closing the sockets the other end will receive an end of file of its sockets
	// this is how the remote end will know that we have disconnected
	m_pNetworkManager->disconnect();
}

Controller::Controller() :
		m_bGameIsRunning(false)
{
	m_pUserInterfaceManager = new UserInterfaceManager();
	m_pNetworkManager = new NetworkManager();
	m_pFalconPenManager = new FalconPenManager();
	m_pZCameraManager = new ZCameraManager();
	m_pSmartClothingManager = new SmartClothingManager();
	m_pGame = new Game();
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

void Controller::notifyNewLocalVideoData(const char* pRGB, unsigned int size)
{
	m_pNetworkManager->sendVideoData(pRGB, size);
}

void Controller::notifyNewRemoteVideoData(const char* pRGB, unsigned int size)
{
	m_pUserInterfaceManager->notifyDisplayNewFrame(pRGB, size);
}

rc_network Controller::sendChatMessage(const std::string& message)
{
	return m_pNetworkManager->sendChatMessage(message);
}

void Controller::notifyNewChatMessage(const std::string& message)
{
	m_pUserInterfaceManager->notifyNewChatMessage(message);
}

void Controller::notifyNewLocalSlingshotPosition(const cVector3d& position)
{
	// update our game with the new slingshot position
	m_pGame->setLocalSlingshotPosition(position);

	// let the peer know that we have moved our slingshot
	m_pNetworkManager->sendSlingshotPosition(position);
}

void Controller::notifyNewRemoteSlingshotPosition(const cVector3d& position)
{
	m_pGame->setRemoteSlingshotPosition(position);
}

void Controller::notifyNewLocalProjectile(const Projectile& projectile)
{
	m_pNetworkManager->sendProjectile(projectile);
	
	m_pGame->addLocalProjectile(projectile);
}

void Controller::notifyNewRemoteProjectile(const Projectile& projectile)
{
	m_pGame->addRemoteProjectile(projectile);
}

void Controller::notifyNewLocalPlayerPosition(const cVector3d& position)
{
	// let our game know that the player has moved
	m_pGame->setLocalPlayerPosition(position);

	// let our peer know that we have moved
	m_pNetworkManager->sendPlayerPosition(position);
}

void Controller::notifyNewRemotePlayerPosition(const cVector3d& position)
{
	// update our game to reflect that our opponent has moved
	m_pGame->setRemotePlayerPosition(position);
}

void Controller::notifyLocalSlingshotPullback()
{
	m_pNetworkManager->sendSlingshotPullback();
}

void Controller::notifyRemoteSlingshotPullback()
{
	// TODO implement
}

void Controller::localStartGame()
{
	// start the game
	startGame();

	// and let the peer know that the game is starting
	m_pNetworkManager->sendStartGame();
}

void Controller::localPauseGame()
{
	// TODO implement
}

void Controller::localExitGame()
{
	// exit the game
	exitGame();

	// and let the peer know that the game is over
	m_pNetworkManager->sendEndGame();
}

void Controller::notifyPeerStartGame()
{
	// start our own game
	startGame();

	// let the UI know that the game has been started
	m_pUserInterfaceManager->notifyGameStarted();
}

void Controller::notifyPeerExitGame()
{
	// exit our own game
	exitGame();

	// let the UI know that the game has been ended
	m_pUserInterfaceManager->notifyGameExited();
}

void Controller::changeArmBandPort(int armBandPort)
{
	//TODO implement
}

void Controller::changeJacketPort(int jacketPort)
{
	// TODO implement
}
