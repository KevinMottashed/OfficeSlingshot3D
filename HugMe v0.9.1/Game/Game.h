#ifndef GAME_H
#define GAME_H

#include "stdafx.h"
#include "chai3d.h"

#include "Projectile.h"
#include "GameStateEnum.h"
#include "MediatorProxy.h"
#include "VirtualEnvironment.h"


/**
 * Represents the OfficeSlingshot3D game.
 * Holds game data (score, health, etc ...)
 * and the games 3d representation.
 */
class Game : public MediatorObserver
{
public:
	/**
	 * Constructor
	 */
	Game(boost::shared_ptr<Mediator> mediator);

	/**
	 * Destructor
	 */
	virtual ~Game();

	/**
	 * Updates from the mediator
	 */
	virtual void update(MediatorUpdateContext_t context, const void* data);

private:
	Game(const Game& game); // intentionally not implemented
	Game& operator=(const Game& game); // intentionally not implemented

	// the main game loop thread
	HANDLE m_hGameLoopThread; // handle
	DWORD m_dwIDGameLoop; // thread id

	/**
	 * This function is the main game loop.
	 * A thread will run this non-stop.
	 */
	static DWORD GameLoopThread(Game* p_Game);

	/**
	 * The current state of the game.
	 */
	GameState_t state;

	/**
	 * The mediator for interpreting user input and synchronizing with the peer
	 */
	boost::shared_ptr<Mediator> mediator;

	/**
	 * The games virtual environment
	 */
	VirtualEnvironment environment;


	/**
	 * Reset the game to its original state.
	 */
	void reset();

	/**
	 * Start the game
	 * @param player the player that started the game (local or peer)
	 */
	void start(Player_t player);

	/**
	 * Pause the game
	 * @param player the player that paused the game (local or peer)
	 */
	void pause(Player_t player);

	/**
	 * Stop the game
	 * @param player the player that exited the game (local or peer)
	 */
	void stop(Player_t player);
};
#endif

