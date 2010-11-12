#ifndef NETWORK_H
#define NETWORK_H

#include "stdafx.h"
#include "chai3d.h"
#include "NetworkSubject.h"
#include "VideoData.h"
#include "NetworkCodes.h"
#include "Projectile.h"
#include "BodyPartEnum.h"
#include "PhysicsSync.h"

/**
 * Abstract class representing the networking component of the application
 * all network interfaces should be able to send and receive data
 */
class Network : public NetworkSubject
{
public:
	/**
	 * Constructor
	 */
	Network();

	/**
	 * Destructor
	 */
	virtual ~Network();

	/**
	 * Start listening for connections
	 * @param userName The username used to establish the connection
	 * @return The error code, SUCCESS if listening was successful
	 */
	virtual rc_network listen(const std::string& userName) = 0;	

	/**
	 * Attempt to connect to a peer
	 * @param ipAddress The ip address that we will connect to
	 * @param userName The username that will be used to establish the connection
	 * @return The error code, SUCCESS if we are now connected to the peer
	 */
	virtual rc_network connect(const std::string& ipAddress, const std::string& userName) = 0;

	/**
	 * Disconnect from a peer
	 * @return The error code, SUCCESS if we successfully disconnected
	 */
	virtual rc_network disconnect() = 0;

	/**
	 * Send a start game message to the peer
	 * @return The error code, SUCCESS if the message was sent and received
	 */
	virtual rc_network sendStartGame() = 0;

	/**
	 * Send a pause game message to the peer
	 * @return The error code, SUCCESS if the message was sent and received
	 */
	virtual rc_network sendPauseGame() = 0;

	/**
	 * Send an end game message to the peer
	 * @return The error code, SUCCESS if the message was sent and received
	 */
	virtual rc_network sendEndGame() = 0;

	/**
	 * Send our username to the peer.
	 * The user name is sent when the connection is established so this should only be used
	 * if a user changes there username
	 * @param userName Our new username that will be sent to the peer
	 * @return The error code, SUCCESS if the message was sent and received
	 */
	virtual rc_network sendUserName(const std::string& userName) = 0;

	/**
	 * Send a chat message to the peer.
	 * @param message The chat message that will be sent to the peer.
	 * @return The error code, SUCCESS if the message was sent and received
	 */
	virtual rc_network sendChatMessage(const std::string& message) = 0;
	
	/**
	 * Send the players position to the peer.
	 * @param position Our player position that will be sent to the peer.
	 * @return The error code, SUCCESS if the message was sent and received
	 */
	virtual rc_network sendPlayerPosition(const cVector3d& position) = 0;

	/**
	 * Send the slingshot pullback position to the peer.
	 * The pullback position is the position of the sling when the slingshot is being fired.
	 * @param position Our slingshot position that will be sent to the peer.
	 * @return The error code, SUCCESS if the message was sent and received
	 */
	virtual rc_network sendSlingshotPullback(const cVector3d& position) = 0;

	/**
	 * Send a projectile to the peer.
	 * @param projectile The projectile that will be sent to the peer.
	 * @return The error code, SUCCESS if the message was sent and received
	 */
	virtual rc_network sendProjectile(const Projectile& projectile) = 0;

	/**
	 * Send a message to the peer letting him know that we have lost the game.
	 * @return The error code, SUCCESS if the message was sent and received
	 */
	virtual rc_network sendGameOver() = 0;

	/**
	 * Send a message to the peer letting him know that we have been hit.
	 * @param bodyPart The body part that was hit.
	 * @return The error code, SUCCESS if the message was sent and received.
	 */
	virtual rc_network sendPlayerHit(BodyPart_t bodyPart) = 0;

	/**
	 * Send a physics sync object to the peer so that he can synchronize his game.
	 * @param sync The sync object.
	 * @return The error code, SUCCESS if the message was sent and received.
	 */
	virtual rc_network sendPhysicsSync(const PhysicsSync& sync) = 0;

	/**
	 * Determines if we are connected to a peer
	 * @return true if we are connected to a peer
	 */	
	virtual bool isConnected() const = 0;

	/**
	 * Determines if we are listening for connections 
	 * @return true if we are listening for connections 
	 */	
	virtual bool isListening() const = 0;
};

#endif
