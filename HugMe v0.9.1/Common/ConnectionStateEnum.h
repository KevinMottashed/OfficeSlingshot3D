#ifndef CONNECTION_STATE_ENUM_H
#define CONNECTION_STATE_ENUM_H

#include "stdafx.h"

namespace ConnectionState
{
	enum ConnectionStateEnum
	{
		LISTENING,
		ESTABLISHING,
		CONNECTED,
		DISCONNECTED
	};

	// get a string from the enum
	std::string lookup(ConnectionStateEnum e);
}

typedef ConnectionState::ConnectionStateEnum ConnectionState_t;

#endif