#ifndef WINSOCK_NETWORK_H
#define WINSOCK_NETWORK_H

#include "Stdafx.h"
#include "boost.h"
#include "NetworkSocket.h"
#include "NetworkCodes.h"
#include "Network.h"
#include "Packet.h"
#include "SyncReaderWriters.h"
#include "VideoData.h"
#include "ConnectionStateEnum.h"

// Forward declarations (files include each other)
class NetworkSocket;

/**
 * The different packet types for a control packet
 */
enum ControlPacketType
{
	CONTROL_PACKET_NAME, /**< a user name packet */
	CONTROL_PACKET_CHAT, /**< a chat message packet */
	CONTROL_PACKET_START_GAME, /**< tell the peer the game is starting */
	CONTROL_PACKET_PAUSE_GAME, /**< tell the peer the game is paused */
	CONTROL_PACKET_END_GAME, /**< tell the peer the game is ending */
	CONTROL_PACKET_UNKNOWN /**< unknown packet type */
};

/**
 * The different packet types for a data packet
 */
enum DataPacketType
{
	DATA_PACKET_VIDEO, /**< Video data */
	DATA_PACKET_PLAYER_POSITION, /**< Player position */
	DATA_PACKET_SLINGSHOT_PULLBACK, /**< Slingshot pullback */
	DATA_PACKET_PROJECTILE, /**< Projectile */
	DATA_PACKET_GAME_OVER, /**< Let the peer know that we have lost the game */
	DATA_PACKET_HEALTH_LOST, /**< Let the peer know that we have lost some health */
	DATA_PACKET_UNKNOWN /**< Unknown packet type */
};

// the 2 packet types
typedef Packet<ControlPacketType> ControlPacket;
typedef Packet<DataPacketType> DataPacket;

/** 
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
	 * This will close any open resources (sockets)
	 */
	virtual ~WinsockNetwork();

	/**
	 * Start listening for connections with the given user name.
	 * @param userName The user name to be used when establishing the connection.
	 * @return error code
	 */
	virtual rc_network listen(const std::string& userName);

	/**
	 * Connect to an ip address with the given user name.
	 * @param ipAddress The ipaddress to connect to.
	 * @param userName The user name to be used when establishing the connection.
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
	 * @param userName Our user name.
	 * @return error code
	 */
	virtual rc_network sendUserName(const std::string& userName);

	/**
	 * Send a chat message to the other player.
	 * @param message The chat message to send.
	 * @return error code.
	 */
	virtual rc_network sendChatMessage(const std::string& message);

	/**
	 * Send a start game message to the peer.
	 * This is used to tell the peer that we have started the game.
	 * @return 
	 */
	virtual rc_network sendStartGame();

	/**
	 * Send a pause game message to the peer.
	 * This is used to tell the peer that we have paused the game.
	 * @return 
	 */
	virtual rc_network sendPauseGame();

	/**
	 * Send an end game message to the peer.
	 * This is used to tell the peer that we have ended the game.
	 * @return 
	 */
	virtual rc_network sendEndGame();

	/**
	 * Send video data to the other player.
	 * @param video The video data to send.
	 * @return error code.
	 */
	virtual rc_network sendVideoData(const VideoData& video);

	/**
	 * Send a player position to the other player.
	 * @param position The position to send.
	 * @return error code.
	 */
	virtual rc_network sendPlayerPosition(const cVector3d& position);

	/**
	 * Send the slingshot pullback position to the peer.
	 * The pullback position is the position of the sling when the slingshot is being fired.
	 * @param position The position to send.
	 * @return error code.
	 */
	virtual rc_network sendSlingshotPullback(const cVector3d& position);

	/**
	 * Send a projectile to the other player.
	 * @param projectile The projectile to send.
	 * @return error code.
	 */
	virtual rc_network sendProjectile(const Projectile& projectile);

	/**
	 * Send a message to the peer letting him know that we have lost the game.
	 * @return The error code, SUCCESS if the message was sent and received
	 */
	virtual rc_network sendGameOver();

	/**
	 * Send a message to the peer letting him know that we have lost some health.
	 * @param healthLost The amount of health that we have lost.
	 * @return The error code, SUCCESS if the message was sent and received.
	 */
	virtual rc_network sendHealthLost(unsigned int healthLost);

	/**
	 * Determines if we are connected to a peer.
	 * @return true if we are connected
	 */
	virtual bool isConnected() const;

	/**
	 * Determines if we are listening for connections.
	 * @return true if we are listening
	 */
	virtual bool isListening() const;

	/**
	 * Notify the network interface that a network connection has been accepted.
	 * This is called by the sockets to let us know when a connection has been accepted.
	 * @param socket The socket that has accepted the connection.
	 */
	void notifyAccept(NetworkSocket* socket);

private:
	//---------------
	// Constants
	//---------------

	// the network ports for our application
	static const int chat_port;
	static const int data_port;

	// maximum size for a control packet
	static const int maximum_control_packet_size;
	static const int maximum_data_packet_size;

	//-----------------------
	// Control Socket
	//-----------------------
	// the control socket, used for sending chat messages, user names, game data ...
	NetworkSocket* m_pControlSocket; // the actual socket
	SOCKET m_hControlSocket; // the socket handle
	BOOL m_bControlConnected; // true if the connection has been accepted
	mutable CRITICAL_SECTION m_csControlSocketSend; // mutex for sending control messages

	// the control receive thread, receives messages through the network
	std::auto_ptr<boost::thread> controlReceiveThread;

	/**
	 * Receive messages from the control socket.
	 * This is an infinite loop and is meant to be run in a thread
	 */
	void controlReceive();

	//-----------------------
	// Data Socket
	//-----------------------
	// the data socket, used for sending data, video, falcon, tactile ...
	NetworkSocket* m_pDataSocket; // the actual socket
	SOCKET m_hDataSocket; // the socket handle
	BOOL m_bDataConnected; // true if the connection has been accepted
	mutable CRITICAL_SECTION m_csDataSocketSend; // mutex for sending data message

	// the data receive thread, receives messages through the network
	std::auto_ptr<boost::thread> dataReceiveThread;

	// the threads managing the data socket
	void dataReceive();

	//--------------------------
	// Private Member functions
	//--------------------------

	// reset the sockets to their original state
	void resetSockets();

	// shutdown the send operation on the sockets
	void shutdownSockets();

	// close the sockets
	void closeSockets();

	// resets variables related to the connection status
	void resetConnectionStatus();

	// initialize the connection, this will create the threads for receiving
	rc_network initializeConnection();

	// establishes the connection,
	// this will send any messages that need to be sent for the connection to be established
	// currently, all we need to send is our user name
	rc_network establishConnection();

	// terminate the network connection and reset everything
	void terminateConnection();

	// send a control message to the peer
	rc_network sendControlMessage(const ControlPacket& message);

	// reset the network connection and notify the observers that the peer has disconnected
	void peerDisconnect();

	// reset the network connection and notify the observers that a network error has occured
	void networkError(rc_network error);

	// send a data message synchronously
	rc_network syncSendDataMessage(const DataPacket& packet);

	// handle a data message
	void handleDataMessage(const DataPacket& message);

	// send a control message synchronously
	rc_network syncSendControlMessage(const ControlPacket& packet);

	// handle a control packet
	void handleControlPacket(const ControlPacket& packet);

	// get and set the connection in a thread safe manner
	ConnectionState_t getConnectionState() const;
	void setConnectionState(ConnectionState_t state);

	//---------------------------
	// Private Data Members
	//---------------------------

	// we need to synchronize the different threads modifying the connection status to avoid race conditions
	// the operations that modify the connection status are:
	// startListening, connect, disconnect, peerDisconnect, notifyAccept and network error
	// only 1 of these operation can be executed at a time or we will run into synchronization issues
	// these operations are considered writers in the readers/writers concurrency pattern
	// the readers are the operations that use the sockets (send/recv)
	mutable SyncReaderWriters m_rwsync_ConnectionStatus;

	bool m_bIsServer; // true if we are the server (listener)
	ConnectionState_t m_connectionState;
	mutable CRITICAL_SECTION m_csConnectionState;

	// true if we are in the process of disconnecting and the disconnect originated from us
	bool m_bLocalDisconnect; 
	mutable CRITICAL_SECTION m_csLocalDisconnect;

	// the user name that we will use to establish a connection to the other player
	std::string userName;

	// the semaphore that will be used to signal a connection established
	mutable CSemaphore* m_sEstablished;
};

#endif

