#ifndef PLAYER_ENUM_H
#define PLAYER_ENUM_H

#include "stdafx.h"

namespace Player
{
	enum PlayerEnum
	{
		LOCAL,
		PEER
	};

	// get a string from the enum
	std::string lookup(PlayerEnum e);
}

typedef Player::PlayerEnum Player_t;

#endif