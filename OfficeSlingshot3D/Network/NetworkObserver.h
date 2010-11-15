#ifndef NETWORK_OBSERVER_H
#define NETWORK_OBSERVER_H

#include "stdafx.h"

/**
 * @ingroup Network
 * @b public
 * The context associated with an update.
 * The context represents what has changed in the observed object.
 */
enum NetworkUpdateContext
{
	PEER_CONNECTED, /**< The peer has connected to us. */
	PEER_DISCONNECTED, /**< The peer has disconnected. */
	PEER_START_GAME, /**< The peer has started the game. */
	PEER_PAUSE_GAME, /**< The peer has paused the game. */
	PEER_EXIT_GAME, /**< The peer has exited the game. */
	NETWORK_ERROR, /**< A network error has occured. */
	RECEIVED_USER_NAME, /**< We have received the peer's user name. */
	RECEIVED_CHAT_MESSAGE, /**< We have received a chat message. */
	RECEIVED_SLINGSHOT_PULLBACK, /**< We have received a slingshot pullback message. */
	RECEIVED_PROJECTILE, /**< We have received a projctile. */
	RECEIVED_PLAYER_POSITION, /**< We have received a player position. */
	RECEIVED_GAME_OVER, /**< We have received a game over. */
	RECEIVED_PLAYER_HIT, /**< We have received a player hit message. */
	RECEIVED_PHYSICS_SYNC, /**< We have received a physics sync message. */
};

/**
 * @ingroup Network
 * @b public
 * Interface for a class observing the network.
 */
class NetworkObserver
{
public:
	NetworkObserver(); /**< Constructor. */
	virtual ~NetworkObserver(); /**< Destructor. */

	/**
	 * Handle a network update.
	 * @param[in] context The update's context, describes what has happened.
	 * @param[in] data The update's data.
	 */
	virtual void update(NetworkUpdateContext context, const void* data) = 0;
};

#endif

