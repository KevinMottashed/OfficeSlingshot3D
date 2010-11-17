#ifndef CONNECTION_STATE_ENUM_H
#define CONNECTION_STATE_ENUM_H

#include "stdafx.h"

/**
 * Namespace for holding the ConnectionState enum.
 * This prevents naming conflicts.
 */
namespace ConnectionState
{
	/**
	 * @ingroup Common
	 * @b public
	 * Enum representing all the different connection states.
	 */
	enum ConnectionStateEnum
	{
		LISTENING, /**< Listening for incoming connections, ie waiting for a peer to connect. */
		ESTABLISHING, /**< Establishing the connection, in this state the players are trading information such as usernames. */
		CONNECTED, /**< Connected to the peer. */
		DISCONNECTED /**< Not connected to anyone. */
	};

	/**
	 * Get a string representation of the enum
	 * @param[in] e The enum to convert to a string
	 * @return The string representation
	 */
	std::string lookup(ConnectionStateEnum e);
}

/**
 * Typedef for convinience
 */
typedef ConnectionState::ConnectionStateEnum ConnectionState_t;

#endif
