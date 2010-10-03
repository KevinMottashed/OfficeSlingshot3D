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
	configuration(configuration)
{
	// create the various components
	smartClothing = shared_ptr<SmartClothingManager>(new SmartClothingManager());

	// attach ourselves as an observer to the components
	network->attach(this);
	userInterface->attach(this);
	zcamera->attach(this);
	falcon->attach(this);

	InitializeCriticalSection(&configurationMutex);
}

Mediator::~Mediator()
{
	DeleteCriticalSection(&configurationMutex);
}

void Mediator::startGame(Player_t player)
{
	if (player == Player::LOCAL)
	{
		rc_network error = network->sendStartGame();
		if (error != SUCCESS)
		{
			handleNetworkError(error);
		}

		// TODO move this to out of this if statement so that music plays for both players
		// For now it is here so that when we test 2 instances on one pc we wont hear 2 copies
		// of the background music.
		// This can be moved over once we add a mute option.
		// play background music and "round 1 fight"
		audio.playGameStart();
		audio.playBGMusic();
	}

	falcon->startPolling();
	zcamera->startCapture();
	userInterface->displayGameStateChanged(GameState::RUNNING, player);
	return;
}

void Mediator::pauseGame(Player_t player)
{
	if (player == Player::LOCAL)
	{
		rc_network error = network->sendPauseGame();
		if (error != SUCCESS)
		{
			handleNetworkError(error);
			return;
		}
	}

	falcon->stopPolling();
	zcamera->stopCapture();
	userInterface->displayGameStateChanged(GameState::PAUSED, player);
	return;
}

void Mediator::exitGame(Player_t player)
{
	if (player == Player::LOCAL)
	{
		rc_network error = network->sendEndGame();
		if (error != SUCCESS)
		{
			handleNetworkError(error);
			return;
		}
	}

	falcon->stopPolling();
	zcamera->stopCapture();
	userInterface->displayGameStateChanged(GameState::NOT_RUNNING, player);
	return;
}

void Mediator::switchCamera(cCamera* camera)
{
	userInterface->switchCamera(camera);
	return;
}

void Mediator::fireSlingshot(Projectile projectile, Player_t player)
{
	// Notify the peer that we fired our slingshot
	if (player == Player::LOCAL)
	{
		rc_network error = network->sendProjectile(projectile);
		if (error != SUCCESS)
		{
			handleNetworkError(error);
			return;
		}

		// Play the slingshot fired sound
		// Only for local player till mute option
		audio.playSlingshotFired();
	}

	return;
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
			Player_t player = Player::PEER;
			notify(MediatorUpdateContext::START_GAME, &player);
			break;
		}
		case PEER_PAUSE_GAME:
		{
			// the peer has paused the game
			Player_t player = Player::PEER;
			notify(MediatorUpdateContext::PAUSE_GAME, &player);
			break;
		}
		case PEER_EXIT_GAME:
		{
			// the peer has exited the game
			Player_t player = Player::PEER;
			notify(MediatorUpdateContext::EXIT_GAME, &player);
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
			// TODO remove video data
			break;
		}
		case RECEIVED_SLINGSHOT_POSITION:
		{
			// the network has received a slingshot position
			assert(data != NULL);

			// adjust the received vector for our perspective
			cVector3d adjusted = *(cVector3d*) data;
			PerspectiveMath::invertPerspective(adjusted);						
			
			notify(MediatorUpdateContext::PEER_SLINGSHOT_MOVED, &adjusted);
			break;
		}
		case RECEIVED_PROJECTILE:
		{
			// the network has received a projectile
			assert(data != NULL);

			// adjust the received vector for our perspective
			Projectile adjusted = *(Projectile*) data;
			PerspectiveMath::invertPerspective(adjusted);

			notify(MediatorUpdateContext::PEER_SLINGSHOT_FIRED, &adjusted);
			break;
		}
		case RECEIVED_PULLBACK:
		{
			// the network has received a slingshot pullback
			notify(MediatorUpdateContext::PEER_PULLBACK_SLINGSHOT);
			break;
		}
		case RECEIVED_PLAYER_POSITION:
		{
			// the network has received a player position
			assert(data != NULL);
			notify(MediatorUpdateContext::PEER_AVATAR_MOVED, data);
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
	notify(MediatorUpdateContext::NETWORK_DISCONNECTED);
	stopDevices();

	// display the change in connection state in the UI
	userInterface->displayConnectionStateChanged(ConnectionState::DISCONNECTED, Player::PEER);
	return;
}

void Mediator::handleNetworkError(rc_network error)
{
	// exit the game when a network error occurs
	notify(MediatorUpdateContext::NETWORK_DISCONNECTED);
	stopDevices();

	// notify the user interface that the network connection as been disconnected
	userInterface->displayNetworkError();
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
			Player_t player = Player::LOCAL;
			notify(MediatorUpdateContext::START_GAME, &player);
			break;
		}
		case PAUSE_GAME:
		{
			Player_t player = Player::LOCAL;
			notify(MediatorUpdateContext::PAUSE_GAME, &player);
			break;
		}
		case EXIT_GAME:
		{
			Player_t player = Player::LOCAL;
			notify(MediatorUpdateContext::EXIT_GAME, &player);
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
	if (network->isConnected() || network->isListening())
	{
		// exit the game when we disconnect
		notify(MediatorUpdateContext::NETWORK_DISCONNECTED);
		stopDevices();

		// tell the network manager to disconnect us
		rc_network error = network->disconnect();

		// we must always be able to disconnect from a peer
		assert(error == SUCCESS);

		// display the change in connection state in the UI
		userInterface->displayConnectionStateChanged(ConnectionState::DISCONNECTED, Player::LOCAL);
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
			// TODO remove video data
			break;
		}
		case AVATAR_POSITION:
		{
			assert(data != NULL);

			// TODO send player position over network
			rc_network error = network->sendPlayerPosition(*(cVector3d*) data);
			if (error != SUCCESS)
			{
				handleNetworkError(error);
				return;
			}
			

			notify(MediatorUpdateContext::LOCAL_AVATAR_MOVED, data);
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

void Mediator::handleLocalVideoData(VideoData& video)
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
		case SLINGSHOT_MOVED:
		{
			assert(data != NULL);

			// let the peer know that we have moved our slingshot
			rc_network error = network->sendSlingshotPosition(*(cVector3d*) data);
			if (error != SUCCESS)
			{
				handleNetworkError(error);
				return;
			}

			notify(MediatorUpdateContext::LOCAL_SLINGSHOT_MOVED, data);
			break;
		}
		case SLINGSHOT_FIRED:
		{
			notify(MediatorUpdateContext::LOCAL_SLINGSHOT_FIRED);
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

void Mediator::stopDevices()
{
	falcon->stopPolling();
	zcamera->stopCapture();
	return;
}