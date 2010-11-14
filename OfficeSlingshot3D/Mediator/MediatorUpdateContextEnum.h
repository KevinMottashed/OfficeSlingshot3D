#ifndef MEDIATOR_UPDATE_CONTEXT_H
#define MEDIATOR_UPDATE_CONTEXT_H

/** 
 * Encapsulates the MediatorUpdateContext enum to avoid naming conflicts.
 */
namespace MediatorUpdateContext
{

/**
 * @ingroup Mediator
 * @b public
 * Each update has a context.
 * The context represents what has changed in the observed object.
 */
enum MediatorUpdateContextEnum
{
	START_GAME, /**< The game has been started. */
	PAUSE_GAME, /**< The game has been paused. */
	EXIT_GAME, /**< The game has been exited. */
	LOCAL_SLINGSHOT_PULLBACK, /**< The local slingshot is being pulled back. */
	PEER_SLINGSHOT_PULLBACK, /**< The peer slingshot is being pulled back. */
	LOCAL_SLINGSHOT_FIRED, /**< The local slingshot has been fired. */
	PEER_SLINGSHOT_FIRED, /**< The peer slingshot has been fired. */
	LOCAL_AVATAR_MOVED, /**< The local avatar moved. */
	PEER_AVATAR_MOVED, /**< The peer avatar moved. */
	NETWORK_DISCONNECTED, /**< The network has been disconnected. */
	PEER_LOST, /**< The peer has lost the game. */
	PEER_HIT, /**< The peer was hit by a projectile. */
	CHANGE_MUTE_ICON, /**< The volume has been muted/unmuted. */
	CHANGE_VOL_ICON, /**< The volume level has changed. */
	PEER_PHYSICS_SYNC, /**< The peer sent a physics sync packet. */
};

}

typedef MediatorUpdateContext::MediatorUpdateContextEnum MediatorUpdateContext_t;


#endif