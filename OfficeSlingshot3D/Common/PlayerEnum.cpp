#include "PlayerEnum.h"

std::string Player::lookup(Player::PlayerEnum e)
{
	switch (e)
	{
		case Player::LOCAL:
		{
			return "player";
		}
		case Player::PEER:
		{
			return "peer";
		}
		default:
		{
			return "unknown";
		}
	}
}