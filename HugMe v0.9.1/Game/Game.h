#ifndef GAME_H
#define GAME_H

#include "stdafx.h"
#include "chai3d.h"

#include "Projectile.h"
#include "GameSubject.h"

class Game : public GameSubject
{
public:
	Game();
	virtual ~Game();

	// alter the game state
	void start();
	void pause();
	void stop();

	// get/set local slingshot position, these are protected by a mutex
	cVector3d getLocalSlingshotPosition() const;
	void setLocalSlingshotPosition(const cVector3d& position);

	// get/set remote slingshot position, these are protected by a mutex
	cVector3d getRemoteSlingshotPosition() const;
	void setRemoteSlingshotPosition(const cVector3d& position);

	// get/set local player position, these are protected by a mutex
	cVector3d getLocalPlayerPosition() const;
	void setLocalPlayerPosition(const cVector3d& position);

	// get/set remote player position, these are protected by a mutex
	cVector3d getRemotePlayerPosition() const;
	void setRemotePlayerPosition(const cVector3d& position);

	// add projectiles to the game
	void addLocalProjectile(const Projectile& projectile);
	void addRemoteProjectile(const Projectile& projectile);

private:
	Game(const Game& game); // intentionally not implemented
	Game& operator=(const Game& game); // intentionally not implemented

	// slingshot position for the players
	// these need to be protected by mutexes because multiple threads will be accessing them
	// you should not access these directly, use the get/set
	cVector3d m_LocalSlingshotPosition;
	mutable CRITICAL_SECTION m_csLocalSlingshotPosition;
	cVector3d m_RemoteSlingshotPosition;
	mutable CRITICAL_SECTION m_csRemoteSlingshotPosition;

	// player positions
	// these need to be protected by mutexes because multiple threads will be accessing them
	// you should not access these directly, use the get/set
	cVector3d m_LocalPlayerPosition;
	mutable CRITICAL_SECTION m_csLocalPlayerPosition;
	cVector3d m_RemotePlayerPosition;
	mutable CRITICAL_SECTION m_csRemotePlayerPosition;

	// vector for storing the local and remote projectiles
	// need to be thread safe because they will be updated from other threads too
	std::vector<Projectile> m_LocalProjectiles;
	mutable CRITICAL_SECTION m_csLocalProjectiles;
	std::vector<Projectile> m_RemoteProjectiles;
	mutable CRITICAL_SECTION m_csRemoteProjectiles;

	// the main game loop thread
	HANDLE m_hGameLoopThread; // handle
	DWORD m_dwIDGameLoop; // thread id

	// this is the thread that controls the game
	static DWORD GameLoopThread(Game* p_Game);

	bool m_bGameIsRunning; // true when the game is running

	// reset the game to its original state
	void reset();
};
#endif

