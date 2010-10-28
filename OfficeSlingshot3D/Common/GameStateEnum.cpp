#include "GameStateEnum.h"

std::string GameState::lookup(GameState::GameStateEnum e)
{
	switch (e)
	{
		case GameState::NOT_RUNNING:
		{
			return "not running";
		}
		case GameState::PAUSED:
		{
			return "paused";
		}
		case GameState::RUNNING:
		{
			return "running";
		}
		default:
		{
			return "unknown";
		}
	}
}