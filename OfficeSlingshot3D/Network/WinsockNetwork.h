#ifndef WINSOCK_NETWORK_H
#define WINSOCK_NETWORK_H

#include "Stdafx.h"
#include "boost.h"
#include "NetworkSocket.h"
#include "NetworkCodes.h"
#include "Network.h"
#include "Packet.h"
#include "SyncReaderWriters.h"
#include "ConnectionStateEnum.h"
#include "BodyPartEnum.h"
#include "PhysicsSync.h"

// Forward declarations (files include each other)
class NetworkSocket;

/**
 * The different packet types for a control packet.
 */
enum ControlPacketType
{
	CONTROL_PACKET_NAME, /**< A user name packet. */
	CONTROL_PACKET_CHAT, /**< A chat message packet. */
	CONTROL_PACKET_START_GAME, /**< Tell the peer the game is starting. */
	CONTROL_PACKET_PAUSE_GAME, /**< Tell the peer the game is paused. */
	CONTROL_PACKET_END_GAME, /**< Tell the peer the game is ending. */
	CONTROL_PACKET_UNKNOWN /**< Unknown packet type. */
};

/**
 * The different packet types for a data packet.
 */
enum DataPacketType
{
	DATA_PACKET_PLAYER_POSITION, /**< Player position. */
	DATA_PACKET_SLINGSHOT_PULLBACK, /**< Slingshot pullback. */
	DATA_PACKET_PROJECTILE, /**< Projectile. */
	DATA_PACKET_GAME_OVER, /**< Let the peer know that we have lost the game. */
	DATA_PACKET_PLAYER_HIT, /**< Let the peer know that we have been hit by a projectile. */
	DATA_PACKET_PHYSICS_SYNC, /**< Give the peer information so that he can synchronize his game to ours. */
	DATA_PACKET_UNKNOWN /**< Unknown packet type. */
};

// the 2 packet types
typedef Packet<ControlPacketType> ControlPacket;
typedef Packet<DataPacketType> DataPacket;

/**
 * @ingroup Network
 * @b public
 * A concrete implementation of the network class using windows sockets.
 * This class uses windows sockets to implement the send/receive functionality.
 */
class WinsockNetwork : public Network 
{
public:
	/**
	 * Constructor.
	 */
	WinsockNetwork();

	/**
	 * Destructor.
	 * This will close any open resources (sockets).
	 */
	virtual ~WinsockNetwork();

	/**
	 * Start listening for connections with the given user name.
	 * @param[in] userName The user name to be used when establishing the connection.
	 * @return error code
	 */
	virtual rc_network listen(const std::string& userName);

	/**
	 * Connect to an ip address with the given user name.
	 * @param[in] ipAddress The ipaddress to connect to.
	 * @param[in] userName The user name to be used when establishing the connection.
	 * @return error code
	 */
	virtual rc_network connect(const std::string& ipAddress, const std::string& userName);

	/**
	 * Disconnect from the peer.
	 * @return error code
	 */
	virtual rc_network disconnect();

	/**
	 * Send our user name to the peer.
	 * @param[in] userName Our user name.
	 * @return The error code, SUCCESS if the message was sent and received.
	 */
	virtual rc_network sendUserName(const std::string& userName);

	/**
	 * Send a chat message to the other player.
	 * @param[in] message The chat message to send.
	 * @return The error code, SUCCESS if the message was sent and received.
	 */
	virtual rc_network sendChatMessage(const std::string& message);

	/**
	 * Send a start game message to the peer.
	 * This is used to tell the peer that we have started the game.
	 * @return The error code, SUCCESS if the message was sent and received. 
	 */
	virtual rc_network sendStartGame();

	/**
	 * Send a pause game message to the peer.
	 * This is used to tell the peer that we have paused the game.
	 * @return The error code, SUCCESS if the message was sent and received. 
	 */
	virtual rc_network sendPauseGame();

	/**
	 * Send an end game message to the peer.
	 * This is used to tell the peer that we have ended the game.
	 * @return The error code, SUCCESS if the message was sent and received.
	 */
	virtual rc_network sendEndGame();

	/**
	 * Send a player position to the other player.
	 * @param[in] position The position to send.
	 * @return The error code, SUCCESS if the message was sent and received.
	 */
	virtual rc_network sendPlayerPosition(const cVector3d& position);

	/**
	 * Send the slingshot pullback position to the peer.
	 * The pullback position is the position of the sling when the slingshot is being fired.
	 * @param[in] position The position to send.
	 * @return The error code, SUCCESS if the message was sent and received.
	 */
	virtual rc_network sendSlingshotPullback(const cVector3d& position);

	/**
	 * Send a projectile to the other player.
	 * @param[in] projectile The projectile to send.
	 * @return The error code, SUCCESS if the message was sent and received.
	 */
	virtual rc_network sendProjectile(const Projectile& projectile);

	/**
	 * Send a message to the peer letting him know that we have lost the game.
	 * @return The error code, SUCCESS if the message was sent and received
	 */
	virtual rc_network sendGameOver();

	/**
	 * Send a message to the peer letting him know that we have been hit.
	 * @param[in] bodyPart The body part that was hit.
	 * @return The error code, SUCCESS if the message was sent and received.
	 */
	virtual rc_network sendPlayerHit(BodyPart_t bodyPart);

	/**
	 * Send a physics sync object to the peer so that he can synchronize his game.
	 * @param[in] sync The sync object.
	 * @return The error code, SUCCESS if the message was sent and received.
	 */
	virtual rc_network sendPhysicsSync(const PhysicsSync& sync);

	/**
	 * Determines if we are connected to a peer.
	 * @return true if we are connected.
	 */
	virtual bool isConnected() const;

	/**
	 * Determines if we are listening for connections.
	 * @return true if we are listening.
	 */
	virtual bool isListening() const;

	/**
	 * Notify the network interface that a network connection has been accepted.
	 * This is called by the sockets to let us know when a connection has been accepted.
	 * @param[in] socket The socket that has accepted the connection.
	 */
	void notifyAccept(NetworkSocket* socket);

private:
	static const int chat_port; /**< Port number for the control socket. */
	static const int data_port; /**< Port number for the data socket. */

	static const int maximum_control_packet_size; /**< Maximum size for a control packet. */
	static const int maximum_data_packet_size; /**< Maximum size for a data packet. */

	//-----------------------
	// Control Socket
	//-----------------------
	// the control socket, used for sending chat messages, user names, game data ...
	NetworkSocket* m_pControlSocket; /**< Socket used for control messages. */
	SOCKET m_hControlSocket; /**< The socket handle for the control socket. */
	BOOL m_bControlConnected; /**< True if the control socket is connected. */

	/** Protects the control socket from trying to send multiple messages at a time. */
	mutable CRITICAL_SECTION m_csControlSocketSend;

	/** The thread for receiving messages through the control socket. */
	std::auto_ptr<boost::thread> controlReceiveThread;

	/**
	 * Receive messages from the control socket.
	 * This is an infinite loop and is meant to be run in a thread.
	 */
	void controlReceive();

	//-----------------------
	// Data Socket
	//-----------------------
	// the data socket, used for sending data, video, falcon, tactile ...
	NetworkSocket* m_pDataSocket; /**< Socket used for data messages. */
	SOCKET m_hDataSocket; /**< The socket handle for the data socket. */
	BOOL m_bDataConnected; /**< True if the data socket is connected. */

	/** Protects the data socket from trying to send multiple messages at a time. */
	mutable CRITICAL_SECTION m_csDataSocketSend;

	/** The thread for receiving messages through the data socket. */
	std::auto_ptr<boost::thread> dataReceiveThread;

	/**
	 * Receive messages from the data socket.
	 * This is an infinite loop and is meant to be run in a thread.
	 */
	void dataReceive();

	//--------------------------
	// Private Member functions
	//--------------------------

	/**
	 * Reset the sockets to their original state.
	 */
	void resetSockets();

	/**
	 * Shutdown the send operation on the sockets.
	 */
	void shutdownSockets();

	/**
	 * Close the sockets.
	 */
	void closeSockets();

	/**
	 * Resets variables related to the connection status.
	 */
	void resetConnectionStatus();

	/**
	 * Initialize the connection, this will create the threads for receiving.
	 */
	rc_network initializeConnection();

	/**
	 * Establishes the connection.
	 * This will send any messages that need to be sent for the connection to be established.
	 * Currently, all we need to send is our user name.
	 */
	rc_network establishConnection();

	/**
	 * Terminate the network connection and reset everything.
	 */
	void terminateConnection();

	/**
	 * Send a control message to the peer.
	 * @param[in] message The control message to send.
	 * @return The error code.
	 */
	rc_network sendControlMessage(const ControlPacket& message);

	/**
	 * Reset the network connection and notify the observers that the peer has disconnected.
	 */
	void peerDisconnect();

	/**
	 * Reset the network connection and notify the observers that a network error has occured.
	 * @param[in] error The error that occured.
	 */
	void networkError(rc_network error);

	/**
	 * Send a data message synchronously.
	 * @param[in] packet The data packet to send.
	 * @return The error code.
	 */
	rc_network syncSendDataMessage(const DataPacket& packet);

	/**
	 * Handle a data message.
	 * @param[in] message The data message to handle.
	 */
	void handleDataMessage(const DataPacket& message);

	/**
	 * Send a control message synchronously.
	 * @param[in] packet The control packet to send.
	 * @return The error code.
	 */
	rc_network syncSendControlMessage(const ControlPacket& packet);

	/**
	 * Handle a control packet.
	 * @param[in] packet The control packet to handle.
	 */
	void handleControlPacket(const ControlPacket& packet);

	/**
	 * Get the connection state.
	 * @return The current state of the connection.
	 */
	ConnectionState_t getConnectionState() const;

	/**
	 * Set the connection state.
	 * @param[in] state The new connection state.
	 */
	void setConnectionState(ConnectionState_t state);

	//---------------------------
	// Private Data Members
	//---------------------------

	/**
	 * We need to synchronize the different threads modifying the connection status to avoid race conditions.
	 * The operations that modify the connection status are:
	 * startListening, connect, disconnect, peerDisconnect, notifyAccept and network error
	 * Only 1 of these operation can be executed at a time or we will run into synchronization issues.
	 * These operations are considered writers in the readers/writers concurrency pattern.
	 * The readers are the operations that use the sockets (send/recv).
	 */
	mutable SyncReaderWriters m_rwsync_ConnectionStatus;

	bool m_bIsServer; /**< True if we are the server (listener). */
	ConnectionState_t m_connectionState; /**< The state of the connection. */
	mutable CRITICAL_SECTION m_csConnectionState; /**< Protects the connection state from concurrent access. */

	/** True if we are in the process of disconnecting and the disconnect originated from us. */
	bool m_bLocalDisconnect;

	/** Protects m_bLocalDisconnect from concurrent access. */
	mutable CRITICAL_SECTION m_csLocalDisconnect;

	/** The user name that we will use to establish a connection to the other player. */
	std::string userName;

	/** The semaphore that will be used to signal a connection established. */
	mutable CSemaphore* m_sEstablished;
};

#endif

