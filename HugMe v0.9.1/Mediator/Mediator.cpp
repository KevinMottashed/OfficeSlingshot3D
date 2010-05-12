#include "Mediator.h"
#include "ConsoleStream.h"


Mediator::Mediator(Network* network) :
	network(network),
	gameState(NOT_PLAYING)
{
	// create the various components
	m_pConfiguration = new Configuration("userPreferences.txt");
	m_pUserInterfaceManager = new UserInterfaceManager(m_pConfiguration->getUserPreferences());
	m_pFalconPenManager = new FalconPenManager();
	m_pZCameraManager = new ZCameraManager();
	m_pSmartClothingManager = new SmartClothingManager();
	m_pGame = new Game();

	// attach ourselves as an observer to the components
	network->attach(this);
	m_pUserInterfaceManager->attach(this);
	m_pZCameraManager->attach(this);
	m_pFalconPenManager->attach(this);
	m_pGame->attach(this);

	// create the logger	
	m_pLogger = new HumanFormatFileLogger("HumanFormat.log");

	// attach the logger to the components
	network->attach(m_pLogger);
	m_pUserInterfaceManager->attach(m_pLogger);
	m_pZCameraManager->attach(m_pLogger);
	m_pFalconPenManager->attach(m_pLogger);
	m_pGame->attach(m_pLogger);

	InitializeCriticalSection(&m_csConfiguration);
}

Mediator::~Mediator()
{
	delete m_pUserInterfaceManager;
	delete m_pFalconPenManager;
	delete m_pZCameraManager;
	delete m_pSmartClothingManager;
	delete m_pGame;
	delete m_pLogger;
	delete m_pConfiguration;
	DeleteCriticalSection(&m_csConfiguration);
}

void Mediator::startGame()
{
	if (gameState == PLAYING)
	{
		return; // already started
	}
	gameState = PLAYING;
	m_pGame->start();
	m_pFalconPenManager->start();
	m_pZCameraManager->start();
}

void Mediator::pauseGame()
{
	gameState = PAUSED;
	m_pGame->pause();
	m_pFalconPenManager->stop();
	m_pZCameraManager->stop();
}

void Mediator::exitGame()
{
	gameState = NOT_PLAYING;
	m_pGame->stop();
	m_pFalconPenManager->stop();
	m_pZCameraManager->stop();
}

CDialog* Mediator::getMainWindow()
{
	return m_pUserInterfaceManager->getMainWindow();
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
			handleNetworkError(*(rc_network*) data);
			break;
		}
		case RECEIVED_USER_NAME:
		{
			// the network has received a user name
			assert(data != NULL);
			handleUserName(*(const std::string*) data);
			break;
		}
		case RECEIVED_CHAT_MESSAGE:
		{
			// the network has received a chat message
			assert(data != NULL);
			handleChatMessage(*(const std::string*) data);
			break;
		}
		case RECEIVED_VIDEO:
		{
			// the network has received some new video
			assert(data != NULL);
			if (gameState == PLAYING)
			{
				handleRemoteVideoData(*(VideoData*) data);
			}
			break;
		}
		case RECEIVED_SLINGSHOT_POSITION:
		{
			// the network has received a slingshot position
			assert(data != NULL);
			if (gameState == PLAYING)
			{
				handleRemoteSlingshotPosition(*(cVector3d*) data);
			}
			break;
		}
		case RECEIVED_PROJECTILE:
		{
			// the network has received a projectile
			assert(data != NULL);
			if (gameState == PLAYING)
			{
				handleRemoteProjectile(*(Projectile*) data);
			}
			break;
		}
		case RECEIVED_PULLBACK:
		{
			// the network has received a slingshot pullback
			if (gameState == PLAYING)
			{
				handleRemotePullback();
			}
			break;
		}
		case RECEIVED_RELEASE:
		{
			// the network has received a slingshot release
			if (gameState == PLAYING)
			{
				handleRemoteRelease();
			}
			break;
		}
		case RECEIVED_PLAYER_POSITION:
		{
			// the network has received a player position
			assert(data != NULL);
			if (gameState == PLAYING)
			{
				handleRemotePlayerPosition(*(cVector3d*) data);
			}
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
	rc_network error = network->connect(prefs.ipAddress, prefs.name);
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
	rc_network error = network->listen(prefs.name);
	
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
	network->disconnect();
	return;
}

void Mediator::changePreferences(const UserPreferences& preferences)
{
	// many threads could try to modify/read the configuration at once so we need to synchronize it
	SyncLocker lock(m_csConfiguration);

	UserPreferences currentPreferences = m_pConfiguration->getUserPreferences();

	if (currentPreferences.name != preferences.name)
	{
		network->sendUserName(preferences.name);
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
	network->sendStartGame();
	return;
}

void Mediator::localPauseGame()
{
	// pause the game
	pauseGame();

	// and let the peer know that the game is paused
	network->sendPauseGame();
	return;
}

void Mediator::localExitGame()
{
	// exit the game
	exitGame();

	// and let the peer know that the game is over
	network->sendEndGame();
	return;
}

void Mediator::closeApplication()
{
	// the only thing to do is to close the sockets so the other end has a clean disconnect
	// by closing the sockets the other end will receive an end of file of its sockets
	// this is how the remote end will know that we have disconnected
	network->disconnect();
	return;
}

void Mediator::sendChatMessage(const std::string& message)
{
	network->sendChatMessage(message);
	return;
}

void Mediator::update(ZCameraUpdateContext context, const void* data)
{
	switch(context)
	{
		case VIDEO:
		{
			assert(data != NULL);
			if (gameState == PLAYING)
			{
				handleLocalVideoData(*(VideoData*) data);
			}
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
	network->sendVideoData(video);
	return;
}

void Mediator::update(FalconUpdateContext context, const void* data)
{
	switch(context)
	{
		case SLINGSHOT_POSITION:
		{
			assert(data != NULL);
			if (gameState == PLAYING)
			{
				handleLocalSlingshotPosition(*(cVector3d*) data);
			}
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

void Mediator::handleLocalSlingshotPosition(const cVector3d& position)
{
	// update our game with the new slingshot position
	m_pGame->setLocalSlingshotPosition(position);

	// let the peer know that we have moved our slingshot
	network->sendSlingshotPosition(position);
	return;
}

void Mediator::update(GameUpdateContext context, const void* data)
{
	// there are currently no updates coming from the game
	assert(false);
	return;
}

