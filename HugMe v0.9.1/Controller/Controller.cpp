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

void Controller::pauseGame()
{
	m_bGameIsRunning = false;
	m_pGame->pause();
	m_pFalconPenManager->stop();
	m_pZCameraManager->stop();
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
	// create the various components
	m_pUserInterfaceManager = new UserInterfaceManager();
	m_pNetworkManager = new NetworkManager();
	m_pFalconPenManager = new FalconPenManager();
	m_pZCameraManager = new ZCameraManager();
	m_pSmartClothingManager = new SmartClothingManager();
	m_pGame = new Game();

	// attach ourselves as an observer to the components
	m_pNetworkManager->attach(this);
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

void Controller::notifyNewLocalVideoData(VideoData video)
{
	m_pUserInterfaceManager->notifyDisplayNewLocalFrame(video);
	m_pNetworkManager->sendVideoData(video);
}

rc_network Controller::sendChatMessage(const std::string& message)
{
	return m_pNetworkManager->sendChatMessage(message);
}

void Controller::notifyNewLocalSlingshotPosition(const cVector3d& position)
{
	// update our game with the new slingshot position
	m_pGame->setLocalSlingshotPosition(position);

	// let the peer know that we have moved our slingshot
	m_pNetworkManager->sendSlingshotPosition(position);
}

void Controller::notifyNewLocalProjectile(const Projectile& projectile)
{
	m_pNetworkManager->sendProjectile(projectile);
	
	m_pGame->addLocalProjectile(projectile);
}

void Controller::notifyNewLocalPlayerPosition(const cVector3d& position)
{
	// let our game know that the player has moved
	m_pGame->setLocalPlayerPosition(position);

	// let our peer know that we have moved
	m_pNetworkManager->sendPlayerPosition(position);
}

void Controller::notifyLocalSlingshotPullback()
{
	m_pNetworkManager->sendSlingshotPullback();
}

void Controller::notifyLocalSlingshotRelease()
{
	m_pNetworkManager->sendSlingshotRelease();
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
	// pause the game
	pauseGame();

	// and let the peer know that the game is paused
	m_pNetworkManager->sendPauseGame();
}

void Controller::localExitGame()
{
	// exit the game
	exitGame();

	// and let the peer know that the game is over
	m_pNetworkManager->sendEndGame();
}

void Controller::changeArmBandPort(int armBandPort)
{
	//TODO implement
}

void Controller::changeJacketPort(int jacketPort)
{
	// TODO implement
}

void Controller::networkUpdate(NetworkUpdateContext context, void* data)
{
	switch (context)
	{
		case PEER_CONNECTED:
		{
			// the peer has connected
			handlePeerConnected();
			break;
		}
		case PEER_DISCONNECTED:
		{
			// the peer has disconnected
			handlePeerDisconnected();
			break;
		}
		case PEER_START_GAME:
		{
			// the peer has started the game
			handlePeerStartGame();
			break;
		}
		case PEER_PAUSE_GAME:
		{
			// the peer has paused the game
			handlePeerPauseGame();
			break;
		}
		case NETWORK_ERROR:
		{
			// a network error has occured
			assert(data != NULL);
			rc_network* error = (rc_network*) data;
			handleNetworkError(*error);
			break;
		}
		case RECEIVED_USER_NAME:
		{
			// the network has received a user name
			assert(data != NULL);
			const std::string* name = (const std::string*) data;
			handleUserName(*name);
			break;
		}
		case RECEIVED_CHAT_MESSAGE:
		{
			// the network has received a chat message
			assert(data != NULL);
			const std::string* message = (const std::string*) data;
			handleChatMessage(*message);
			break;
		}
		case RECEIVED_VIDEO:
		{
			// the network has received some new video
			assert(data != NULL);
			VideoData* video = (VideoData*) data;
			handleRemoteVideoData(*video);
			break;
		}
		case RECEIVED_SLINGSHOT_POSITION:
		{
			// the network has received a slingshot position
			assert(data != NULL);
			cVector3d* position = (cVector3d*) data;
			handleRemoteSlingshotPosition(*position);
			break;
		}
		case RECEIVED_PROJECTILE:
		{
			// the network has received a projectile
			assert(data != NULL);
			Projectile* projectile = (Projectile*) data;
			handleRemoteProjectile(*projectile);
			break;
		}
		case RECEIVED_PULLBACK:
		{
			// the network has received a slingshot pullback
			handleRemotePullback();
			break;
		}
		case RECEIVED_RELEASE:
		{
			// the network has received a slingshot release
			handleRemoteRelease();
			break;
		}
		case RECEIVED_PLAYER_POSITION:
		{
			// the network has received a player position
			assert(data != NULL);
			cVector3d* position = (cVector3d*) data;
			handleRemotePlayerPosition(*position);
			break;
		}
		default:
		{
			// All updates should be handled
			assert(false);
			break;
		}
	}
	return;
}

void Controller::handlePeerConnected()
{
	// send the connection accepted message to the user interface process
	m_pUserInterfaceManager->notifyNetworkConnectionEstablished();
	return;
}

void Controller::handlePeerDisconnected()
{
	// exit the game when the peer disconnects
	exitGame();

	// notify the user interface that the network connection has been disconnected
	m_pUserInterfaceManager->notifyPeerDisconnected();
	return;
}

void Controller::handleNetworkError(rc_network error)
{
	// exit the game when a network error occurs
	exitGame();

	// notify the user interface that the network connection as been disconnected
	m_pUserInterfaceManager->notifyNetworkError(error);
	return;
}

void Controller::handlePeerStartGame()
{
	// start our own game
	startGame();

	// let the UI know that the game has been started
	m_pUserInterfaceManager->notifyGameStarted();
	return;
}

void Controller::handlePeerPauseGame()
{
	// pause our game
	pauseGame();

	// let the UI know that the game has been paused
	m_pUserInterfaceManager->notifyGamePaused();
	return;
}

void Controller::handlePeerExitGame()
{
	// exit our own game
	exitGame();

	// let the UI know that the game has been ended
	m_pUserInterfaceManager->notifyGameExited();
	return;
}

void Controller::handleUserName(const std::string& name)
{
	// update the other player's user name
	remoteUserName = name;
	return;
}

void Controller::handleChatMessage(const std::string& message)
{
	// tell the UI to display the chat message
	m_pUserInterfaceManager->notifyNewChatMessage(message);
	return;
}

void Controller::handleRemoteVideoData(VideoData video)
{
	// tell the UI to display the video
	m_pUserInterfaceManager->notifyDisplayNewRemoteFrame(video);
	return;
}

void Controller::handleRemoteSlingshotPosition(const cVector3d& position)
{
	// update the slingshot position in the game module
	m_pGame->setRemoteSlingshotPosition(position);
	return;
}

void Controller::handleRemoteProjectile(const Projectile& projectile)
{
	// add this new projectile to the game
	m_pGame->addRemoteProjectile(projectile);
	return;
}

void Controller::handleRemotePlayerPosition(const cVector3d& position)
{
	// update our game to reflect that our opponent has moved
	m_pGame->setRemotePlayerPosition(position);
	return;
}

void Controller::handleRemotePullback()
{
	// TODO implement
	return;
}

void Controller::handleRemoteRelease()
{
	// TODO implement
	return;
}
