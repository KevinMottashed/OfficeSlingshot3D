// Game.cpp: implementation of the Game class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Game.h"
#include "SyncLocker.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Game::Game(Controller* pController) :
	m_LocalSlingshotPosition(cVector3d(0,0,0)),
	m_RemoteSlingshotPosition(cVector3d(0,0,0)),
	m_LocalPlayerPosition(cVector3d(0,0,0)),
	m_RemotePlayerPosition(cVector3d(0,0,0)),
	m_pController(pController),
	m_bGameIsRunning(false)
{
	InitializeCriticalSection(&m_csLocalSlingshotPosition);
	InitializeCriticalSection(&m_csRemoteSlingshotPosition);
	InitializeCriticalSection(&m_csLocalPlayerPosition);
	InitializeCriticalSection(&m_csRemotePlayerPosition);
}

Game::~Game()
{
	DeleteCriticalSection(&m_csLocalSlingshotPosition);
	DeleteCriticalSection(&m_csRemoteSlingshotPosition);
	DeleteCriticalSection(&m_csLocalPlayerPosition);
	DeleteCriticalSection(&m_csRemotePlayerPosition);
}

cVector3d Game::getLocalSlingshotPosition() const
{
	SyncLocker lock(m_csLocalPlayerPosition);
	return m_LocalSlingshotPosition;
}

void Game::setLocalSlingshotPosition(const cVector3d& position)
{
	SyncLocker lock(m_csLocalPlayerPosition);
	m_LocalSlingshotPosition = position;
}

cVector3d Game::getRemoteSlingshotPosition() const
{
	SyncLocker lock(m_csRemoteSlingshotPosition);
	return m_RemoteSlingshotPosition;
}

void Game::setRemoteSlingshotPosition(const cVector3d& position)
{
	SyncLocker lock(m_csRemoteSlingshotPosition);
	m_RemoteSlingshotPosition = position;
}

cVector3d Game::getLocalPlayerPosition() const
{
	SyncLocker lock(m_csLocalPlayerPosition);
	return m_LocalPlayerPosition;
}

void Game::setLocalPlayerPosition(const cVector3d& position)
{
	SyncLocker lock(m_csLocalPlayerPosition);
	m_LocalPlayerPosition = position;
}

cVector3d Game::getRemotePlayerPosition() const
{
	SyncLocker lock(m_csRemotePlayerPosition);
	return m_RemotePlayerPosition;
}

void Game::setRemotePlayerPosition(const cVector3d& position)
{
	SyncLocker lock(m_csRemotePlayerPosition);
	m_RemotePlayerPosition = position;
}

void Game::start()
{
	if (m_bGameIsRunning)
	{
		// dont try to start the game twice
		return;
	}
	m_bGameIsRunning = true;
	m_hGameLoopThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) GameLoopThread, (void*) this, 0, &m_dwIDGameLoop);
	return;
}

void Game::stop()
{
	m_bGameIsRunning = false;
	m_hGameLoopThread = 0;
	m_dwIDGameLoop = 0;

	// reset the game
	reset();

	return;
}

void Game::reset()
{
	// reset all the positions to the default
	setLocalPlayerPosition(cVector3d(0,0,0));
	setRemotePlayerPosition(cVector3d(0,0,0));
	setLocalSlingshotPosition(cVector3d(0,0,0));
	setRemoteSlingshotPosition(cVector3d(0,0,0));
	return;
}

DWORD Game::GameLoopThread(Game* pGame)
{
	while(pGame->m_bGameIsRunning)
	{
		// slingshot positions
		std::cout << "The local slingshot is at " << pGame->getLocalSlingshotPosition() <<  std::endl;
		std::cout << "The remote slingshot is at " << pGame->getRemoteSlingshotPosition() <<  std::endl;

		// player positions
		std::cout << "The local player is at " << pGame->getLocalPlayerPosition() <<  std::endl;
		std::cout << "The remote player is at " << pGame->getRemotePlayerPosition() <<  std::endl;

		// sleep for a short while
		Sleep(1000); // 1 sec
	}
	return 0;
}
