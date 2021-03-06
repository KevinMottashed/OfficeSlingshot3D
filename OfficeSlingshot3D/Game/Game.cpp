#include "stdafx.h"
#include "Game.h"
#include "SyncLocker.h"
#include "PhysicsSync.h"

using namespace std;
using namespace boost;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Game::Game(boost::shared_ptr<Mediator> mediator) :
	mediator(mediator),
	state(GameState::NOT_RUNNING),
	age(0)
{
	// observe the mediator
	mediator->attach(this);

	environment.initialize();
	
	mediator->setVolumePreferences();
	mediator->switchCamera(titleScreen.camera());

	// create the thread that runs the main game loop
	gameThread = auto_ptr<thread>(new thread(boost::bind(&Game::gameLoop, this)));
}

Game::~Game()
{
	if (gameThread.get() != NULL)
	{
		// kill the thread
		gameThread->interrupt();
		gameThread->join();
		gameThread.reset();
	}
}

void Game::update(MediatorUpdateContext_t context, const void* data)
{
	boost::mutex::scoped_lock lock(updateQueueMutex);

	switch (context)
	{
		// all updates that have no data
		case MediatorUpdateContext::LOCAL_SLINGSHOT_FIRED:
		case MediatorUpdateContext::NETWORK_DISCONNECTED:
		case MediatorUpdateContext::PEER_LOST:
		{
			updateQueue.push(std::pair<MediatorUpdateContext_t, boost::any>(context, boost::any()));
			break;
		}

		// all updates where data is of type Player_t
		case MediatorUpdateContext::START_GAME:
		case MediatorUpdateContext::PAUSE_GAME:
		case MediatorUpdateContext::EXIT_GAME:
		{
			updateQueue.push(std::pair<MediatorUpdateContext_t, boost::any>(context, *(Player_t*) data));
			break;
		}

		// all updates where data is of type cVector3d 
		case MediatorUpdateContext::LOCAL_SLINGSHOT_PULLBACK:
		case MediatorUpdateContext::PEER_SLINGSHOT_PULLBACK:
		case MediatorUpdateContext::LOCAL_AVATAR_MOVED:
		case MediatorUpdateContext::PEER_AVATAR_MOVED:
		{
			updateQueue.push(std::pair<MediatorUpdateContext_t, boost::any>(context, *(cVector3d*) data));
			break;
		}

		// all updates where data is of type Projectile 
		case MediatorUpdateContext::PEER_SLINGSHOT_FIRED:
		{
			updateQueue.push(std::pair<MediatorUpdateContext_t, boost::any>(context, *(Projectile*) data));
			break;
		}

		// all updates where data is of type BodyPart_t
		case MediatorUpdateContext::PEER_HIT:
		{
			updateQueue.push(std::pair<MediatorUpdateContext_t, boost::any>(context, *(BodyPart_t*) data));
			break;
		}

		// all updates where data is of type bool
		case MediatorUpdateContext::CHANGE_MUTE_ICON:
		{
			updateQueue.push(std::pair<MediatorUpdateContext_t, boost::any>(context, *(bool*) data));
			break;
		}

		// all updates where data is of type int
		case MediatorUpdateContext::CHANGE_VOL_ICON:
		{
			updateQueue.push(std::pair<MediatorUpdateContext_t, boost::any>(context, *(int*) data));
			break;
		}

		// all updates where data is of type PhysicsSync
		case MediatorUpdateContext::PEER_PHYSICS_SYNC:
		{
			updateQueue.push(std::pair<MediatorUpdateContext_t, boost::any>(context, *(PhysicsSync*) data));
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
	age = 0;
	environment.resetAll();
	return;
}

void Game::gameLoop()
{
	unsigned int updatesSinceLastSync = 0;

	// keep going until the thread is interrupted,
	// We will check for an interruption after each step in the loop.
	// There are 2 reasons for doing this.
	// 1. Some steps are time consuming and we want to interrupt as fast as possible.
	// 2. If the application is being closed the UI will no longer respond to paint requests (could crash or deadlock).
	while(true)
	{
		// handle all the updates in the queue
		processUpdateQueue();

		// check for an interruption
		boost::this_thread::interruption_requested();

		// update game environment if it is needed
		if (state == GameState::RUNNING)
		{
			// update the environment, check for collisions, etc..
			updateWorld();

			// we want to sync with the peer every 5 "regular" updates.
			// as opposed to updates imposed upon us by received physics sync packets
			++updatesSinceLastSync;
			if (updatesSinceLastSync == 5)
			{
				// send a physics sync packet to the peer so that the games stay synchronised.
				sendPhysicsSync();
				updatesSinceLastSync = 0;
			}			

			// check for an interruption
			boost::this_thread::interruption_requested();
		}

		// We repaint the game after each iteration so that the user can see the changes.
		// Even if the game isnt running we still want to repaint the screen. If we didn't repaint
		// the screen then the 3d environment wouldn't be shown on startup or after a minimize.
		mediator->paint();

		// yield to other threads (interruption point)
		boost::this_thread::sleep(boost::posix_time::milliseconds(20));
	}
}

void Game::processUpdateQueue()
{
	boost::mutex::scoped_lock lock(updateQueueMutex);

	while (!updateQueue.empty())
	{
		std::pair<MediatorUpdateContext_t, boost::any>& update = updateQueue.front();
		handleUpdate(update.first, update.second);
		updateQueue.pop();
	}
}

void Game::handleUpdate(MediatorUpdateContext_t& context, boost::any& data)
{
	if (isWorldRelatedUpdate(context) && state != GameState::RUNNING)
	{
		// we don't handle world related updates (avatar positions, projectiles, etc...)
		// while the game isn't running
		return;
	}

	switch (context)
	{
		case MediatorUpdateContext::START_GAME:
		{
			start(boost::any_cast<Player_t>(data));
			break;
		}
		case MediatorUpdateContext::PAUSE_GAME:
		{
			pause(boost::any_cast<Player_t>(data));
			break;
		}
		case MediatorUpdateContext::EXIT_GAME:
		{
			stop(boost::any_cast<Player_t>(data));
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
		case MediatorUpdateContext::LOCAL_SLINGSHOT_PULLBACK:
		{
			cVector3d relBallPos = boost::any_cast<cVector3d>(data);
			environment.pullBackLocalBall(relBallPos);
			break;
		}
		case MediatorUpdateContext::PEER_SLINGSHOT_PULLBACK:
		{
			cVector3d relBallPos = boost::any_cast<cVector3d>(data);
			environment.pullBackPeerBall(relBallPos);
			break;
		}
		case MediatorUpdateContext::LOCAL_SLINGSHOT_FIRED:
		{
			// fire the slingshot and get the resulting projectile
			Projectile p = environment.fireLocalSlingshot();

			// Tell the mediator to provide adequate feedback for when the slingshot is fired
			mediator->fireSlingshot(p, Player::LOCAL);
			break;
		}
		case MediatorUpdateContext::PEER_SLINGSHOT_FIRED:
		{
			Projectile p = boost::any_cast<Projectile>(data);
			environment.firePeerSlingshot(p);

			// Tell the mediator to provide adequate feedback for when the slingshot is fired
			mediator->fireSlingshot(p, Player::PEER);
			break;
		}
		case MediatorUpdateContext::LOCAL_AVATAR_MOVED:
		{
			cVector3d pos = boost::any_cast<cVector3d>(data);
			environment.moveLocalAvatar(pos);
			break;
		}
		case MediatorUpdateContext::PEER_AVATAR_MOVED:
		{
			cVector3d pos = boost::any_cast<cVector3d>(data);
			environment.movePeerAvatar(pos);
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
			BodyPart_t bodyPart = boost::any_cast<BodyPart_t>(data);
			unsigned int dmg = calculateHitDamage(bodyPart);
			environment.reducePeerHp(dmg);
			break;
		}
		case MediatorUpdateContext::CHANGE_MUTE_ICON:
		{
			bool soundOn = boost::any_cast<bool>(data);
			environment.changeMuteIcon(soundOn);
			titleScreen.changeMuteIcon(soundOn);
			loseScreen.changeMuteIcon(soundOn);
			winScreen.changeMuteIcon(soundOn);
			break;
		}
		case MediatorUpdateContext::CHANGE_VOL_ICON:
		{
			int vol = boost::any_cast<int>(data);
			environment.changeVolumeIcon(vol);
			titleScreen.changeVolumeIcon(vol);
			loseScreen.changeVolumeIcon(vol);
			winScreen.changeVolumeIcon(vol);
			break;
		}
		case MediatorUpdateContext::PEER_PHYSICS_SYNC:
		{
			PhysicsSync sync = boost::any_cast<PhysicsSync>(data);
			handlePhysicsSync(sync);
			break;
		}
	}
}

void Game::updateWorld()
{
	// increment the age of the game, the age goes up once per frame
	++age;

	// update the environment, this will move the projectiles
	environment.updateFrame();			

	// check for collisions and handle them
	checkForCollisions();
	return;
}

void Game::checkForCollisions()
{
	if (environment.isColliding()) 
	{
		cVector3d ballPos = environment.getCurrentBallPosition();
		BodyPart_t bodyPart = environment.getAvatarHitBodyPart();
		unsigned int damage = calculateHitDamage(bodyPart);
		switch(bodyPart) {
		
		case BodyPart::CHEST:
			mediator->collisionDetected(BodyPart::CHEST, ballPos, environment.getLocalAvatarChestMin(), environment.getLocalAvatarChestMax());
			environment.reduceLocalHp(damage);
			break;
		case BodyPart::UPPER_RIGHT_ARM:
			mediator->collisionDetected(BodyPart::UPPER_RIGHT_ARM, ballPos, environment.getLocalAvatarRightArmMin(), environment.getLocalAvatarRightArmMax());
			environment.reduceLocalHp(damage);
			break;
		case BodyPart::UPPER_LEFT_ARM:
			mediator->collisionDetected(BodyPart::UPPER_LEFT_ARM, ballPos, environment.getLocalAvatarLeftArmMin(), environment.getLocalAvatarLeftArmMax());
			environment.reduceLocalHp(damage);
			break;
		case BodyPart::HEAD:
			mediator->collisionDetected(BodyPart::HEAD, ballPos, environment.getLocalAvatarHeadMin(), environment.getLocalAvatarHeadMax());
			environment.reduceLocalHp(damage);
			break;
		}
		
		//Check if the local player is dead, if he is, we notify the mediator and change camera to loseScreen
		if (environment.isLocalPlayerDead())
		{					
			mediator->playerLost(Player::LOCAL);
			mediator->switchCamera(loseScreen.camera());
			reset();
		}
	}
}

void Game::sendPhysicsSync()
{
	// gather all information related to the physics simulation
	PhysicsSync sync;
	sync.age(age);

	mediator->synchronizePhysics(sync);
}

void Game::handlePhysicsSync(const PhysicsSync& sync)
{
	unsigned long peerAge = sync.age();

	// Fast forward our game if we have fallen behind.
	// We advance our game to peerAge - 1. The "-1" is there so that
	// we don't fast forward past the peer. The updateWorld() that will sync us with the
	// peer (age==peerAge) will be done in the main game loop
	while (peerAge - 1 > age)
	{
		updateWorld();		
	}

	// We don't care if we are ahead of the peer. 
	// It's the peer's responsibility to catch up.

	return;
}

unsigned int Game::calculateHitDamage(BodyPart_t bodyPart)
{
	switch(bodyPart)
	{
	case BodyPart::HEAD:
		return 3;
	case BodyPart::CHEST:
		return 2;
	case BodyPart::UPPER_LEFT_ARM:
	case BodyPart::UPPER_RIGHT_ARM:
		return 1;
	}
	assert(false); // should handle all body parts
	return 0;
}

bool Game::isWorldRelatedUpdate(MediatorUpdateContext_t context)
{
	switch(context)
	{
	case MediatorUpdateContext::LOCAL_SLINGSHOT_PULLBACK:
	case MediatorUpdateContext::PEER_SLINGSHOT_PULLBACK:
	case MediatorUpdateContext::LOCAL_SLINGSHOT_FIRED:
	case MediatorUpdateContext::PEER_SLINGSHOT_FIRED:
	case MediatorUpdateContext::LOCAL_AVATAR_MOVED:
	case MediatorUpdateContext::PEER_AVATAR_MOVED:
	case MediatorUpdateContext::PEER_LOST:
	case MediatorUpdateContext::PEER_HIT:
	case MediatorUpdateContext::PEER_PHYSICS_SYNC:
		return true;
	default:
		return false;
	}
}

