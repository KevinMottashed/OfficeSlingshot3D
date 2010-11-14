#ifndef LOG_EVENT_H
#define LOG_EVENT_H

#include "stdafx.h"

/**
 * Namespace that holds the log event enum.
 * This prevents naming conflicts.
 */
namespace LogEvent
{
	/**
	 * @ingroup Logger
	 * @b package
	 * This enum represents all input events that the system can receive.
	 * These include inputs from the network, the user interface, the falcon and the zcamera.
	 */
	enum LogEventEnum
	{
		// network events (0-99)
		START_NETWORK = 0, /**< The first of the network events. */
		NETWORK_PEER_CONNECTED = START_NETWORK, /**< A peer has connected to us. */
		NETWORK_PEER_DISCONNECTED, /**< The peer has disconnected. */
		NETWORK_PEER_START_GAME, /**< The peer has started the game. */
		NETWORK_PEER_PAUSE_GAME, /**< The peer has paused the game. */
		NETWORK_PEER_EXIT_GAME, /**< The peer has exited the game. */
		NETWORK_ERROR_OCCURED, /**< A network error occured. */
		NETWORK_USER_NAME, /**< We received a user name over the network. */
		NETWORK_CHAT_MESSAGE, /**< We received a chat message. */
		NETWORK_SLINGSHOT_PULLBACK, /**< We received a slingshot pullback packet. */
		NETWORK_PROJECTILE, /**< We received a projectile packet. */
		NETWORK_PLAYER_POSITION, /**< We received a player position. */
		NETWORK_GAME_OVER, /**< We received a game over. */
		NETWORK_PLAYER_HIT, /**< We received a player hit packet. */
		NETWORK_PHYSICS_SYNC, /**< We received a physics sync packet. */
		END_NETWORK = 100, /**< Last network event +1. */

		// UI events (100-199)	
		START_UI = END_NETWORK, /**< The first of the ui events. */
		UI_CONNECT = START_UI, /**< The player wants to connect to a peer. */
		UI_LISTEN, /**< The player wants to listen for connections. */
		UI_DISCONNECT, /**< The player wants to disconnect from the peer. */
		UI_PREFERENCES, /**< The player changed his preferences. */
		UI_START_GAME, /**< The player wants to start the game. */
		UI_PAUSE_GAME, /**< The player wants to pause the game. */
		UI_EXIT_GAME, /**< The player wants to exit the game. */
		UI_CLOSE_APPLICATION, /**< The player wants to close the application. */
		UI_CHAT_MESSAGE, /**< The player wants to send a chat message. */
		END_UI = 200, /**< The last of the ui events +1. */

		// Falcon events (200-299)
		START_FALCON = END_UI, /**< The first of the falcon events. */
		FALCON_SLINGSHOT_PULLBACK = START_FALCON, /**< The slingshot is being pulled back. */
		FALCON_SLINGSHOT_FIRED, /**< The slingshot was fired. */
		END_FALCON = 300, /**< The last of the falcon events +1. */

		// ZCam events (300-399)
		START_ZCAM = END_FALCON, /**< The first of the zcamera events. */
		ZCAM_AVATAR_POSITION = START_ZCAM, /**< The player has moved. */
		END_ZCAM = 400, /**< The last of the zcamera events +1. */
	};

	/**
	 * Convert the enum to a string.
	 * @param[in] logEvent The enum to convert to a string.
	 * @return The converted enum.
	 */
	std::string lookup(LogEventEnum logEvent);
}

typedef LogEvent::LogEventEnum LogEvent_t;

#endif
