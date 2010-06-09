#ifndef GAME_STATE_ENUM_H
#define GAME_STATE_ENUM_H

#include "stdafx.h"

namespace GameState
{

	enum GameStateEnum
	{
		NOT_RUNNING,
		PAUSED,
		RUNNING
	};

	// get a string from the enum
	std::string lookup(GameStateEnum e);
}

typedef GameState::GameStateEnum GameState_t;

#endif