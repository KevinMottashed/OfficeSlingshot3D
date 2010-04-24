// Mediator.cpp: implementation of the Mediator class.
//
//////////////////////////////////////////////////////////////////////

#include "Mediator.h"
#include "ConsoleStream.h"

Mediator* Mediator::globalInstance = NULL;

Mediator* Mediator::instance()
{
	if (globalInstance == NULL)
	{
		globalInstance = new Mediator();
	}
	return globalInstance;
}

void Mediator::startGame()
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

void Mediator::pauseGame()
{
	m_bGameIsRunning = false;
	m_pGame->pause();
	m_pFalconPenManager->stop();
	m_pZCameraManager->stop();
}

void Mediator::exitGame()
{
	m_bGameIsRunning = false;
	m_pGame->stop();
	m_pFalconPenManager->stop();
	m_pZCameraManager->stop();
}

Mediator::Mediator() :
		m_bGameIsRunning(false)
{
	// create the various components
	m_pConfiguration = new Configuration("userPreferences.txt");
	m_pUserInterfaceManager = new UserInterfaceManager(m_pConfiguration->getUserPreferences());
	m_pNetworkManager = new NetworkManager();
	m_pFalconPenManager = new FalconPenManager();
	m_pZCameraManager = new ZCameraManager();
	m_pSmartClothingManager = new SmartClothingManager();
	m_pGame = new Game();

	// attach ourselves as an observer to the components
	m_pNetworkManager->attach(this);
	m_pUserInterfaceManager->attach(this);
	m_pZCameraManager->attach(this);

	// create the logger
	m_pLogger = new ConsoleLogger();

	// attach the logger to the components
	m_pNetworkManager->attach(m_pLogger);

	InitializeCriticalSection(&m_csConfiguration);
}

Mediator::~Mediator()
{
	delete m_pNetworkManager;
	delete m_pUserInterfaceManager;
	delete m_pFalconPenManager;
	delete m_pZCameraManager;
	delete m_pSmartClothingManager;
	delete m_pGame;
	delete m_pLogger;
	delete m_pConfiguration;
	DeleteCriticalSection(&m_csConfiguration);
}

CDialog* Mediator::getMainWindow()
{
	return m_pUserInterfaceManager->getMainWindow();
}

void Mediator::notifyNewLocalSlingshotPosition(const cVector3d& position)
{
	// update our game with the new slingshot position
	m_pGame->setLocalSlingshotPosition(position);

	// let the peer know that we have moved our slingshot
	m_pNetworkManager->sendSlingshotPosition(position);
}

void Mediator::notifyNewLocalProjectile(const Projectile& projectile)
{
	m_pNetworkManager->sendProjectile(projectile);
	
	m_pGame->addLocalProjectile(projectile);
}

void Mediator::notifyNewLocalPlayerPosition(const cVector3d& position)
{
	// let our game know that the player has moved
	m_pGame->setLocalPlayerPosition(position);

	// let our peer know that we have moved
	m_pNetworkManager->sendPlayerPosition(position);
}

void Mediator::notifyLocalSlingshotPullback()
{
	m_pNetworkManager->sendSlingshotPullback();
}

void Mediator::notifyLocalSlingshotRelease()
{
	m_pNetworkManager->sendSlingshotRelease();
}

void Mediator::update(NetworkUpdateContext context, const void* data)
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
		case PEER_EXIT_GAME:
		{
			// the peer has exited the game
			handlePeerExitGame();
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

void Mediator::handlePeerConnected()
{
	// send the connection accepted message to the user interface process
	m_pUserInterfaceManager->displayConnectionEstablished();
	return;
}

void Mediator::handlePeerDisconnected()
{
	// exit the game when the peer disconnects
	exitGame();

	// notify the user interface that the network connection has been disconnected
	m_pUserInterfaceManager->displayPeerDisconnected();
	return;
}

void Mediator::handleNetworkError(rc_network error)
{
	// exit the game when a network error occurs
	exitGame();

	// notify the user interface that the network connection as been disconnected
	m_pUserInterfaceManager->displayNetworkError();
	return;
}

void Mediator::handlePeerStartGame()
{
	// start our own game
	startGame();

	// let the UI know that the game has been started
	m_pUserInterfaceManager->displayGameStarted();
	return;
}

void Mediator::handlePeerPauseGame()
{
	// pause our game
	pauseGame();

	// let the UI know that the game has been paused
	m_pUserInterfaceManager->displayGamePaused();
	return;
}

void Mediator::handlePeerExitGame()
{
	// exit our own game
	exitGame();

	// let the UI know that the game has been ended
	m_pUserInterfaceManager->displayGameExited();
	return;
}

void Mediator::handleUserName(const std::string& name)
{
	// update the other player's user name
	m_pUserInterfaceManager->setPeerUserName(name);
	return;
}

void Mediator::handleChatMessage(const std::string& message)
{
	// tell the UI to display the chat message
	m_pUserInterfaceManager->displayPeerChatMessage(message);
	return;
}

void Mediator::handleRemoteVideoData(VideoData video)
{
	// tell the UI to display the video
	m_pUserInterfaceManager->displayRemoteFrame(video);
	return;
}

void Mediator::handleRemoteSlingshotPosition(const cVector3d& position)
{
	// update the slingshot position in the game module
	m_pGame->setRemoteSlingshotPosition(position);
	return;
}

void Mediator::handleRemoteProjectile(const Projectile& projectile)
{
	// add this new projectile to the game
	m_pGame->addRemoteProjectile(projectile);
	return;
}

void Mediator::handleRemotePlayerPosition(const cVector3d& position)
{
	// update our game to reflect that our opponent has moved
	m_pGame->setRemotePlayerPosition(position);
	return;
}

void Mediator::handleRemotePullback()
{
	// TODO implement
	return;
}

void Mediator::handleRemoteRelease()
{
	// TODO implement
	return;
}

void Mediator::update(UserInterfaceUpdateContext context, const void* data)
{
	switch (context)
	{
		case CONNECT:
		{
			connect();
			break;
		}
		case LISTEN:
		{
			listen();
			break;
		}
		case DISCONNECT:
		{
			disconnect();
			break;
		}
		case PREFERENCES:
		{
			assert(data != NULL);
			changePreferences(*(UserPreferences*) data);
			break;
		}
		case START_GAME:
		{
			localStartGame();
			break;
		}
		case PAUSE_GAME:
		{
			localPauseGame();
			break;
		}
		case EXIT_GAME:
		{
			localExitGame();
			break;
		}
		case EXIT_APPLICATION:
		{
			closeApplication();
			break;
		}
		case CHAT_MESSAGE:
		{
			assert(data != NULL);
			sendChatMessage(*(std::string*) data);
			break;
		}
		default:
		{
			// all updates should be handled
			assert(false);
			break;
		}
	}
	return;
}

void Mediator::connect()
{
	// many threads could try to modify/read the configuration at once so we need to synchronize it
	SyncLocker lock(m_csConfiguration);

	UserPreferences prefs = m_pConfiguration->getUserPreferences();
	rc_network error = m_pNetworkManager->connect(prefs.ipAddress, prefs.name);
	if (error == SUCCESS)
	{
		m_pUserInterfaceManager->displayConnectionEstablished();
	}
	else
	{
		m_pUserInterfaceManager->displayConnectionFailed();
	}
	return;
}

void Mediator::listen()
{
	// many threads could try to modify/read the configuration at once so we need to synchronize it
	SyncLocker lock(m_csConfiguration);

	UserPreferences prefs = m_pConfiguration->getUserPreferences();
	rc_network error = m_pNetworkManager->listen(prefs.name);
	
	if (error == SUCCESS)
	{
		m_pUserInterfaceManager->displayListening();
	}
	else
	{
		m_pUserInterfaceManager->displayFailedToListen();
	}

	return;
}

// we want to exit the game and disconnect when the local user wishes to disconnect
void Mediator::disconnect()
{
	// exit the game when we disconnect
	exitGame();

	// tell the network manager to disconnect us
	m_pNetworkManager->disconnect();
	return;
}

void Mediator::changePreferences(const UserPreferences& preferences)
{
	// many threads could try to modify/read the configuration at once so we need to synchronize it
	SyncLocker lock(m_csConfiguration);

	UserPreferences currentPreferences = m_pConfiguration->getUserPreferences();

	if (currentPreferences.name != preferences.name)
	{
		m_pNetworkManager->sendUserName(preferences.name);
	}

	if (currentPreferences.armBandPort != preferences.armBandPort)
	{
		// TODO, notify smart clothing manager
	}

	if (currentPreferences.jacketPort != preferences.jacketPort)
	{
		// TODO, notify smart clothing manager
	}

	m_pConfiguration->setUserPreferences(preferences);
	return;
}

void Mediator::localStartGame()
{
	// start the game
	startGame();

	// and let the peer know that the game is starting
	m_pNetworkManager->sendStartGame();
	return;
}

void Mediator::localPauseGame()
{
	// pause the game
	pauseGame();

	// and let the peer know that the game is paused
	m_pNetworkManager->sendPauseGame();
	return;
}

void Mediator::localExitGame()
{
	// exit the game
	exitGame();

	// and let the peer know that the game is over
	m_pNetworkManager->sendEndGame();
	return;
}

void Mediator::closeApplication()
{
	// the only thing to do is to close the sockets so the other end has a clean disconnect
	// by closing the sockets the other end will receive an end of file of its sockets
	// this is how the remote end will know that we have disconnected
	m_pNetworkManager->disconnect();
	return;
}

void Mediator::sendChatMessage(const std::string& message)
{
	m_pNetworkManager->sendChatMessage(message);
	return;
}

void Mediator::update(ZCameraUpdateContext context, const void* data)
{
	switch(context)
	{
		case VIDEO:
		{
			assert(data != NULL);
			handleLocalVideoData(*(VideoData*) data);
			break;
		}
		default:
		{
			// all updates should be handled
			assert(false);
		}
	}
	return;
}

void Mediator::handleLocalVideoData(VideoData video)
{
	m_pUserInterfaceManager->displayLocalFrame(video);
	m_pNetworkManager->sendVideoData(video);
	return;
}
