#ifndef NETWORK_H
#define NETWORK_H

#include "NetworkSubject.h"
#include "stdafx.h"
#include "VideoData.h"
#include "chai3d.h"
#include "NetworkCodes.h"
#include "Projectile.h"

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
	 * Send video data to the peer.
	 * @param video The video data that will be sent to the peer.
	 * @return The error code, SUCCESS if the message was sent and received
	 */
	virtual rc_network sendVideoData(const VideoData& video) = 0;

	/**
	 * Send the players position to the peer.
	 * @param position Our player position that will be sent to the peer.
	 * @return The error code, SUCCESS if the message was sent and received
	 */
	virtual rc_network sendPlayerPosition(const cVector3d& position) = 0;

	/**
	 * Send the slingshot position to the peer.
	 * @param position Our slingshot position that will be sent to the peer.
	 * @return The error code, SUCCESS if the message was sent and received
	 */
	virtual rc_network sendSlingshotPosition(const cVector3d& position) = 0;

	/**
	 * Send a projectile to the peer.
	 * @param projectile The projectile that will be sent to the peer.
	 * @return The error code, SUCCESS if the message was sent and received
	 */
	virtual rc_network sendProjectile(const Projectile& projectile) = 0;

	/**
	 * Send a slingshot pullback event to the peer.
	 * This lets the peer know that we have started to pull back our slingshot.
	 * @return The error code, SUCCESS if the message was sent and received
	 */
	virtual rc_network sendSlingshotPullback() = 0;

	/**
	 * Send a message to the peer letting him know that we have lost the game.
	 * @return The error code, SUCCESS if the message was sent and received
	 */
	virtual rc_network sendGameOver() = 0;

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
