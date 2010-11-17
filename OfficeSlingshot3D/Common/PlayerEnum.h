#ifndef PLAYER_ENUM_H
#define PLAYER_ENUM_H

#include "stdafx.h"

/**
 * Namespace for holding the Player enum.
 * This prevents naming conflicts.
 */
namespace Player
{
	/**
	 * @ingroup Common
	 * @b public
	 * Enum representing all the different players.
	 */
	enum PlayerEnum
	{
		LOCAL, /**< The local player. */
		PEER /**< The remote player. */
	};

	/**
	 * Get a string representation of the enum
	 * @param[in] e The enum to convert to a string
	 * @return The string representation
	 */
	std::string lookup(PlayerEnum e);
}

/**
 * Typedef for convinience.
 */
typedef Player::PlayerEnum Player_t;

#endif
