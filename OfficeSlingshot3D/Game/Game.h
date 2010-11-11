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
#include "WinScreen.h"
#include "LoseScreen.h"
#include "BodyPartEnum.h"


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
	 * Updates from the mediator.
	 * All these updates are placed in the update queue which the game loop will handle.
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
	 * Helper for the game loop.
	 * This will handle all updates in the updateQueue.
	 */
	void processUpdateQueue();

	/**
	 * Handle a single update.
	 */
	void handleUpdate(MediatorUpdateContext_t& context, boost::any& data);

	/**
	 * Helper for the game loop.
	 * This will check for collisions and handle them.
	 */
	void checkForCollisions();

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
	 * The game's win screen.
	 * This is a 3d environment that is displayed when the game is over and the local player is the winner
	 */
	WinScreen winScreen;

	/**
	 * The game's lose screen.
	 * This is a 3d environment that is displayed when the game is over and the local player is the loser
	 */
	LoseScreen loseScreen;

	/**
	 * The game's update queue.
	 * All game affecting updates are added to this queue so that they can be processed one at a time.
	 */
	std::queue<std::pair<MediatorUpdateContext_t, boost::any> > updateQueue;

	/**
	 * Protect the update queue from simultaneous access.
	 */
	boost::mutex updateQueueMutex;

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

	/**
	 * Calculate the damage done when a player is hit on a certain body part.
	 * @param bodyPart The body part where the player was hit.
	 * @return The damage done
	 */
	static unsigned int calculateHitDamage(BodyPart_t bodyPart);
};
#endif

