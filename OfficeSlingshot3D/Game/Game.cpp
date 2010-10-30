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
		case MediatorUpdateContext::PEER_LOST:
		{
			mediator->switchCamera(winScreen.camera());
			reset();
			break;
		}
		case MediatorUpdateContext::PEER_HIT:
		{
			assert(data != NULL);
			environment.reducePeerHp(*(unsigned int*) data);
			break;
		}
		case MediatorUpdateContext::CHANGE_MUTE_ICON:
		{
			assert(data != NULL);
			environment.changeMuteIcon(*(bool*) data);
			titleScreen.changeMuteIcon(*(bool*) data);
			loseScreen.changeMuteIcon(*(bool*) data);
			winScreen.changeMuteIcon(*(bool*) data);
			break;
		}
		case MediatorUpdateContext::CHANGE_VOL_ICON:
		{
			assert(data != NULL);
			environment.changeVolumeIcon(*(bool*) data);
			titleScreen.changeVolumeIcon(*(bool*) data);
			loseScreen.changeVolumeIcon(*(bool*) data);
			winScreen.changeVolumeIcon(*(bool*) data);
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
		
		// The join should be uncommented but it causes issues.
		// We would need to add an update handler thread to uncomment this.
		// TODO: kevin, add the update handler.
		//gameThread->join();

		gameThread.reset();
		environment.resetAll();
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
				cVector3d ballPos = environment.getCurrentBallPosition();
				switch(environment.getAvatarHitBodyPart()) {
				
				case CHEST:
					mediator->collisionDetected(CHEST, ballPos, environment.getLocalAvatarChestMin(), environment.getLocalAvatarChestMax(), 2);
					environment.reduceLocalHp(2);
					break;
				case RIGHT_UPPER_ARM:
					mediator->collisionDetected(RIGHT_UPPER_ARM, ballPos, environment.getLocalAvatarRightArmMin(), environment.getLocalAvatarRightArmMax(), 1);
					environment.reduceLocalHp(1);
					break;
				case LEFT_UPPER_ARM:
					mediator->collisionDetected(LEFT_UPPER_ARM, ballPos, environment.getLocalAvatarLeftArmMin(), environment.getLocalAvatarLeftArmMax(), 1);
					environment.reduceLocalHp(1);
					break;
				case HEAD:
					mediator->collisionDetected(HEAD, ballPos, environment.getLocalAvatarHeadMin(), environment.getLocalAvatarHeadMax(), 3);
					environment.reduceLocalHp(3);
					break;
				}
				
				//Check if the local player is dead, if he is, we notify the mediator and change camera to loseScreen
				if (environment.isLocalPlayerDead())
				{					
					mediator->playerLost(Player::LOCAL);
					mediator->switchCamera(loseScreen.camera());
					reset();
					return;
				}
			}
		} // release the lock before sleeping

		// yield to other threads (interruption point)
		boost::this_thread::sleep(boost::posix_time::milliseconds(1));
	}
}

