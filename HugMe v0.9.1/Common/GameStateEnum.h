#ifndef GAME_STATE_ENUM_H
#define GAME_STATE_ENUM_H

#include "stdafx.h"

/**
 * Namespace for holding the GameState enum.
 * This prevents naming conflicts.
 */
namespace GameState
{
	/**
	 * Enum representing all the different game states
	 */
	enum GameStateEnum
	{
		NOT_RUNNING, /**< The game is not currently running */
		PAUSED, /**< The game is paused */
		RUNNING /**< The game is running */
	};
	
	/**
	 * Get a string representation of the enum
	 * @param e The enum to convert to a string
	 * @return The string representation
	 */
	std::string lookup(GameStateEnum e);
}

/**
 * Typedef for convinience
 */
typedef GameState::GameStateEnum GameState_t;

#endif
