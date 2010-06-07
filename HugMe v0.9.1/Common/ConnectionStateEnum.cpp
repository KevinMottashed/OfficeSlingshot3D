#include "ConnectionStateEnum.h"

std::string ConnectionState::lookup(ConnectionState::ConnectionStateEnum e)
{
	switch (e)
	{
		case ConnectionState::LISTENING:
		{
			return "listening";
		}
		case ConnectionState::ESTABLISHING:
		{
			return "establishing";
		}
		case ConnectionState::CONNECTED:
		{
			return "connected";
		}		
		case ConnectionState::DISCONNECTED:
		{
			return "disconnected";
		}
		default:
		{
			return "unknown";
		}
	}
}