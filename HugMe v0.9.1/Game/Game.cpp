#include "stdafx.h"
#include "Game.h"
#include "SyncLocker.h"

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
	
	mediator->switchCamera(environment.camera());
}

Game::~Game()
{
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
			break;
		}
		case MediatorUpdateContext::LOCAL_SLINGSHOT_MOVED:
		{
			assert(data != NULL);
			environment.moveLocalSlingshot(*(cVector3d*) data);
			break;
		}
		case MediatorUpdateContext::PEER_SLINGSHOT_MOVED:
		{
			assert(data != NULL);
			environment.movePeerSlingshot(*(cVector3d*) data);
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
	m_hGameLoopThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) GameLoopThread, (void*) this, 0, &m_dwIDGameLoop);

	// tell the mediator that the game has started so that it can start managing the devices
	mediator->startGame(player);

	return;
}

// a pause is the same thing as a stop
// except that we don't reset the game state
// that way we can resume from the previous state
void Game::pause(Player_t player)
{
	state = GameState::PAUSED;
	m_hGameLoopThread = 0;
	m_dwIDGameLoop = 0;

	// tell the mediator that the game has been paused so that it can stop managing the devices
	mediator->pauseGame(player);

	return;
}

void Game::stop(Player_t player)
{
	// tell the mediator that the game has been stopped so that it can stop managing the devices
	mediator->exitGame(player);

	// reset the game
	reset();
	return;
}

void Game::reset()
{
	state = GameState::NOT_RUNNING;
	m_hGameLoopThread = 0;
	m_dwIDGameLoop = 0;
	return;
}

DWORD Game::GameLoopThread(Game* pGame)
{
	while(pGame->state == GameState::RUNNING)
	{
		pGame->environment.updateFrame();

		// sleep for a short while
		Sleep(100); // 0.1 secs / 10FPS
	}
	return 0;
}

