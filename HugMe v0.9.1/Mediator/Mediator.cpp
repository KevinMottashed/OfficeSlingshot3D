#include "Mediator.h"
#include "ConsoleStream.h"

using namespace std;
using namespace boost; 


Mediator::Mediator(boost::shared_ptr<Network> network,
				   boost::shared_ptr<Falcon> falcon,
				   boost::shared_ptr<IZCamera> zcamera,
				   boost::shared_ptr<UserInterface> userInterface,
				   boost::shared_ptr<Configuration> configuration) :
	network(network),
	userInterface(userInterface),
	falcon(falcon),
	zcamera(zcamera),
	game(),
	configuration(configuration),
	gameState(GameState::NOT_RUNNING)
{
	// create the various components
	smartClothing = shared_ptr<SmartClothingManager>(new SmartClothingManager());

	// attach ourselves as an observer to the components
	network->attach(this);
	userInterface->attach(this);
	zcamera->attach(this);
	falcon->attach(this);
	game.attach(this);

	InitializeCriticalSection(&configurationMutex);
}

Mediator::~Mediator()
{
	DeleteCriticalSection(&configurationMutex);
}

void Mediator::startGame()
{
	if (gameState == GameState::RUNNING)
	{
		return; // already started
	}
	gameState = GameState::RUNNING;
	game.start();
	falcon->startPolling();
	zcamera->startCapture();
}

void Mediator::pauseGame()
{
	if (gameState != GameState::RUNNING)
	{
		return; // nothing to do, the game is either already paused or not started
	}
	gameState = GameState::PAUSED;
	game.pause();
	falcon->stopPolling();
	zcamera->stopCapture();
}

void Mediator::exitGame()
{
	if (gameState == GameState::NOT_RUNNING)
	{
		return; // nothing to do, the game is not running
	}
	gameState = GameState::NOT_RUNNING;
	game.stop();
	falcon->stopPolling();
	zcamera->stopCapture();
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
			if (gameState == GameState::RUNNING)
			{
				handleRemoteVideoData(*(VideoData*) data);
			}
			break;
		}
		case RECEIVED_SLINGSHOT_POSITION:
		{
			// the network has received a slingshot position
			assert(data != NULL);
			if (gameState == GameState::RUNNING)
			{
				handleRemoteSlingshotPosition(*(cVector3d*) data);
			}
			break;
		}
		case RECEIVED_PROJECTILE:
		{
			// the network has received a projectile
			assert(data != NULL);
			if (gameState == GameState::RUNNING)
			{
				handleRemoteProjectile(*(Projectile*) data);
			}
			break;
		}
		case RECEIVED_PULLBACK:
		{
			// the network has received a slingshot pullback
			if (gameState == GameState::RUNNING)
			{
				handleRemotePullback();
			}
			break;
		}
		case RECEIVED_RELEASE:
		{
			// the network has received a slingshot release
			if (gameState == GameState::RUNNING)
			{
				handleRemoteRelease();
			}
			break;
		}
		case RECEIVED_PLAYER_POSITION:
		{
			// the network has received a player position
			assert(data != NULL);
			if (gameState == GameState::RUNNING)
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
	// display the change in connection state in the UI
	userInterface->displayConnectionStateChanged(ConnectionState::CONNECTED, Player::PEER);
	return;
}

void Mediator::handlePeerDisconnected()
{
	// exit the game when the peer disconnects
	exitGame();

	// display the change in connection state in the UI
	userInterface->displayConnectionStateChanged(ConnectionState::DISCONNECTED, Player::PEER);
	return;
}

void Mediator::handleNetworkError(rc_network error)
{
	// exit the game when a network error occurs
	exitGame();

	// notify the user interface that the network connection as been disconnected
	userInterface->displayNetworkError();
	return;
}

void Mediator::handlePeerStartGame()
{
	// start our own game
	startGame();

	// let the UI know that the game has been started
	userInterface->displayGameStateChanged(GameState::RUNNING, Player::PEER);
	return;
}

void Mediator::handlePeerPauseGame()
{
	// pause our game
	pauseGame();

	// let the UI know that the game has been paused
	userInterface->displayGameStateChanged(GameState::PAUSED, Player::PEER);
	return;
}

void Mediator::handlePeerExitGame()
{
	// exit our own game
	exitGame();

	// let the UI know that the game has been ended
	userInterface->displayGameStateChanged(GameState::NOT_RUNNING, Player::PEER);
	return;
}

void Mediator::handleUserName(const std::string& name)
{
	// update the other player's user name
	userInterface->setPeerUserName(name);
	return;
}

void Mediator::handleChatMessage(const std::string& message)
{
	// tell the UI to display the chat message
	userInterface->displayPeerChatMessage(message);
	return;
}

void Mediator::handleRemoteVideoData(VideoData video)
{
	// tell the UI to display the video
	userInterface->displayRemoteFrame(video);
	return;
}

void Mediator::handleRemoteSlingshotPosition(const cVector3d& position)
{
	// update the slingshot position in the game module
	game.setRemoteSlingshotPosition(position);
	return;
}

void Mediator::handleRemoteProjectile(const Projectile& projectile)
{
	// add this new projectile to the game
	game.addRemoteProjectile(projectile);
	return;
}

void Mediator::handleRemotePlayerPosition(const cVector3d& position)
{
	// update our game to reflect that our opponent has moved
	game.setRemotePlayerPosition(position);
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
	SyncLocker lock(configurationMutex);

	UserPreferences prefs = configuration->getUserPreferences();
	rc_network error = network->connect(prefs.ipAddress, prefs.name);

	if (error == SUCCESS)
	{
		// display the change in connection state in the UI
		userInterface->displayConnectionStateChanged(ConnectionState::CONNECTED, Player::LOCAL);
	}
	else
	{
		userInterface->displayConnectionFailed();
	}
	return;
}

void Mediator::listen()
{
	// many threads could try to modify/read the configuration at once so we need to synchronize it
	SyncLocker lock(configurationMutex);

	UserPreferences prefs = configuration->getUserPreferences();
	rc_network error = network->listen(prefs.name);
	
	if (error == SUCCESS)
	{
		// display the change in connection state in the UI
		userInterface->displayConnectionStateChanged(ConnectionState::LISTENING, Player::LOCAL);
	}
	else
	{
		userInterface->displayFailedToListen();
	}

	return;
}

// we want to exit the game and disconnect when the local user wishes to disconnect
void Mediator::disconnect()
{
	if (network->isConnected())
	{
		// exit the game when we disconnect
		exitGame();

		// tell the network manager to disconnect us
		rc_network error = network->disconnect();

		if (error == SUCCESS)
		{
			// display the change in connection state in the UI
			userInterface->displayConnectionStateChanged(ConnectionState::DISCONNECTED, Player::LOCAL);
		}
		else
		{
			// this should never happen, we must always be able to disconnect from a peer
			assert(false);
		}
	}	
	return;
}

void Mediator::changePreferences(const UserPreferences& preferences)
{
	// many threads could try to modify/read the configuration at once so we need to synchronize it
	SyncLocker lock(configurationMutex);

	UserPreferences currentPreferences = configuration->getUserPreferences();

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

	configuration->setUserPreferences(preferences);
	return;
}

void Mediator::localStartGame()
{
	rc_network code = network->sendStartGame();
	if (code == SUCCESS)
	{
		// start the game
		startGame();

		userInterface->displayGameStateChanged(GameState::RUNNING, Player::LOCAL);
	}
	return;
}

void Mediator::localPauseGame()
{
	rc_network code = network->sendPauseGame();
	if (code == SUCCESS)
	{
		// pause the game
		pauseGame();

		userInterface->displayGameStateChanged(GameState::PAUSED, Player::LOCAL);
	}
	return;
}

void Mediator::localExitGame()
{
	rc_network code = network->sendEndGame();
	if (code == SUCCESS)
	{
		// pause the game
		exitGame();

		userInterface->displayGameStateChanged(GameState::NOT_RUNNING, Player::LOCAL);
	}
	return;
}

void Mediator::closeApplication()
{
	// the only thing to do is to close the sockets so the other end has a clean disconnect
	// by closing the sockets the other end will receive an end of file of its sockets
	// this is how the remote end will know that we have disconnected
	// the disconnect function will also exit the game
	// by disconnecting the network and exiting the game all threads will have (or will soon be) exited
	// so the application will be closed
	disconnect();
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
			if (gameState == GameState::RUNNING)
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
	network->sendVideoData(video);
	ZCamera::reverseFrameLeftRight(video,4);
	userInterface->displayLocalFrame(video);
	return;
}

void Mediator::update(FalconUpdateContext context, const void* data)
{
	switch(context)
	{
		case SLINGSHOT_POSITION:
		{
			assert(data != NULL);
			if (gameState == GameState::RUNNING)
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
	game.setLocalSlingshotPosition(position);

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

