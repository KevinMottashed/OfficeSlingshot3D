#ifndef MEDIATOR_UPDATE_CONTEXT_H
#define MEDIATOR_UPDATE_CONTEXT_H

/** 
 * Encapsulates the MediatorUpdateContext enum to avoid naming conflicts
 */
namespace MediatorUpdateContext
{

/** 
 * Each update has a context.
 * The context represents what has changed in the observed object
 */
enum MediatorUpdateContextEnum
{
	START_GAME,
	PAUSE_GAME,
	EXIT_GAME,
	PEER_SLINGSHOT_MOVED,
	PEER_AVATAR_MOVED,
	PEER_FIRED_PROJECTILE,
	PEER_PULLBACK_SLINGSHOT,
	LOCAL_SLINGSHOT_MOVED,
	LOCAL_AVATAR_MOVED,
	NETWORK_DISCONNECTED,
};

}

typedef MediatorUpdateContext::MediatorUpdateContextEnum MediatorUpdateContext_t;


#endif