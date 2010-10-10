#include "stdafx.h"
#include "Game.h"
#include "SyncLocker.h"

using namespace std;
using namespace boost;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Game::Game(boost::shared_ptr<Mediator> mediator) :
	mediator(mediator),
	state(GameState::NOT_RUNNING)
{
	// observe the mediator
	mediator->attach(this);

	environment.initialize();
	
	mediator->switchCamera(titleScreen.camera());
}

Game::~Game()
{
	if (gameThread.get() != NULL)
	{
		// kill the thread if its still running
		gameThread->interrupt();
		gameThread->join();
		gameThread.reset();
	}
}

void Game::update(MediatorUpdateContext_t context, const void* data)
{
	switch (context)
	{
		case MediatorUpdateContext::START_GAME:
		{
			start(*(Player_t*) data);
			break;
		}
		case MediatorUpdateContext::PAUSE_GAME:
		{
			pause(*(Player_t*) data);
			break;
		}
		case MediatorUpdateContext::EXIT_GAME:
		{
			stop(*(Player_t*) data);
			break;
		}
		case MediatorUpdateContext::NETWORK_DISCONNECTED:
		{
			// reset the game
			reset();

			// change camera to the title screen environment
			mediator->switchCamera(titleScreen.camera());
			break;
		}
		case MediatorUpdateContext::LOCAL_SLINGSHOT_MOVED:
		{
			assert(data != NULL);

			// we can't update the game and change it at the same time
			boost::mutex::scoped_lock lock(environment_mutex);

			environment.moveLocalSlingshot(*(cVector3d*) data);
			break;
		}
		case MediatorUpdateContext::PEER_SLINGSHOT_MOVED:
		{
			assert(data != NULL);
			
			// we can't update the game and change it at the same time
			boost::mutex::scoped_lock lock(environment_mutex);

			environment.movePeerSlingshot(*(cVector3d*) data);
			break;
		}
		case MediatorUpdateContext::LOCAL_SLINGSHOT_FIRED:
		{
			// we can't update the game and change it at the same time
			boost::mutex::scoped_lock lock(environment_mutex);

			// fire the slingshot and get the resulting projectile
			Projectile p = environment.fireLocalSlingshot();

			// Tell the mediator to provide adequate feedback for when the slingshot is fired
			mediator->fireSlingshot(p, Player::LOCAL);
			break;
		}
		case MediatorUpdateContext::PEER_SLINGSHOT_FIRED:
		{
			assert(data != NULL);

			// we can't update the game and change it at the same time
			boost::mutex::scoped_lock lock(environment_mutex);

			Projectile p = *(Projectile*) data;
			environment.firePeerSlingshot(p);

			// Tell the mediator to provide adequate feedback for when the slingshot is fired
			mediator->fireSlingshot(p, Player::PEER);
			break;
		}
		case MediatorUpdateContext::LOCAL_AVATAR_MOVED:
		{
			assert(data != NULL);
			environment.moveLocalAvatar(*(cVector3d*) data);
			break;
		}
		case MediatorUpdateContext::PEER_AVATAR_MOVED:
		{
			assert(data != NULL);
			environment.movePeerAvatar(*(cVector3d*) data);
			break;
		}
	}
}

void Game::start(Player_t player)
{
	if (state == GameState::RUNNING)
	{
		// dont try to start the game twice
		return;
	}
	state = GameState::RUNNING;
	
	// make sure we arn't trying to start it twice
	assert(gameThread.get() == NULL);

	// create the thread that runs the main game loop
	gameThread = auto_ptr<thread>(new thread(boost::bind(&Game::gameLoop, this)));

	// tell the mediator that the game has started so that it can start managing the devices
	mediator->startGame(player);

	// change camera to the virtual environment
	mediator->switchCamera(environment.camera());

	return;
}

// a pause is the same thing as a stop
// except that we don't reset the game state
// that way we can resume from the previous state
void Game::pause(Player_t player)
{
	// make sure it's actually running
	if (state != GameState::RUNNING)
	{
		return;
	}
	state = GameState::PAUSED;

	// tell the mediator that the game has been paused so that it can stop managing the devices
	mediator->pauseGame(player);

	// the thread should be running if the game is running
	assert(gameThread.get() != NULL);

	// kill the thread
	gameThread->interrupt();
	gameThread->join();
	gameThread.reset();

	return;
}

void Game::stop(Player_t player)
{
	// tell the mediator that the game has been stopped so that it can stop managing the devices
	mediator->exitGame(player);

	// change camera to the title screen environment
	mediator->switchCamera(titleScreen.camera());

	// reset the game
	reset();
	return;
}

void Game::reset()
{
	state = GameState::NOT_RUNNING;

	if (gameThread.get() != NULL)
	{
		// kill the thread
		gameThread->interrupt();
		gameThread->join();
		gameThread.reset();
	}

	return;
}

void Game::gameLoop()
{
	// keep going until the thread is interrupted
	while(true)
	{
		{
			boost::mutex::scoped_lock lock(environment_mutex);
			environment.updateFrame();
			
			if (environment.isColliding()) 
			{
				mediator->collisionDetected();
				environment.reduceLocalHp(1);
			}
		} // release the lock before sleeping

		// yield to other threads (interruption point)
		boost::this_thread::sleep(boost::posix_time::milliseconds(1));
	}
}

