#ifndef GAME_H
#define GAME_H

#include "stdafx.h"
#include "boost.h"
#include "chai3d.h"

#include "Projectile.h"
#include "GameStateEnum.h"
#include "MediatorProxy.h"
#include "VirtualEnvironment.h"
#include "TitleScreen.h"


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
	/**
	 * Copy constructor, not implemented to protect from use
	 */
	Game(const Game& game);
	
	/**
	 * Assignment operator, not implemented to protect from use
	 */
	Game& operator=(const Game& game);

	// the main game loop thread
	HANDLE m_hGameLoopThread; // handle
	DWORD m_dwIDGameLoop; // thread id

	/**
	 * Holds the thread which will run the main game loop
	 */
	std::auto_ptr<boost::thread> gameThread;

	/**
	 * This function is the main game loop.
	 * A thread will run this non-stop.
	 */
	void gameLoop();

	/**
	 * The current state of the game.
	 */
	GameState_t state;

	/**
	 * The mediator for interpreting user input and synchronizing with the peer
	 */
	boost::shared_ptr<Mediator> mediator;

	/**
	 * The games virtual environment.
	 * This is the 3d environment that is displayed when the game is running.
	 */
	VirtualEnvironment environment;

	/**
	 * The game's title screen.
	 * This is a 3d environment that is displayed when the game isn't running
	 */
	TitleScreen titleScreen;

	/**
	 * Mutex to protect the virtual environment
	 * The environment cannot be modified (move slingshot, etc ...)
	 * and displayed (update frame) at the same time without synchronization issues.
	 * This mutex must be locked before doing either of those operations.
	 */
	boost::mutex environment_mutex;

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

