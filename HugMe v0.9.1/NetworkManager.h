// NetworkManager.h: interface for the NetworkManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NETWORKMANAGER_H__3D85BBC3_3F80_477F_ABAB_1DAE8326532A__INCLUDED_)
#define AFX_NETWORKMANAGER_H__3D85BBC3_3F80_477F_ABAB_1DAE8326532A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Controller.h"
#include "NetworkSocket.h"
#include "NetworkCodes.h"
#include "DataPacket.h"
#include "ControlPacket.h"
#include "Stdafx.h"

// Forward declarations (files include each other)
class Controller;
class NetworkSocket;

class NetworkManager  
{
public:
	NetworkManager();
	virtual ~NetworkManager();

	// start listening for connections
	rc_network startListening();

	// connect to a host
	rc_network connect(const std::string& ipAdrress);

	// disconnect from a host
	rc_network disconnect();

	// return true if we are connected to a peer
	bool isConnected() const;

	// send the player's user name over the network
	rc_network sendUserName(const std::string& userName);

	// send a chat message to the other player
	rc_network sendChatMessage(const std::string& message);

	// send a start game message
	rc_network sendStartGame();

	// send a end game message
	rc_network sendEndGame();

	// send a video only packet to the other player
	rc_network sendVideoData(const std::vector<BYTE>& vRGB);

	// notify the controller that a network connection has been accepted
	void notifyAccept(NetworkSocket* socket);

private:
	//---------------
	// Constants
	//---------------

	// the network ports for our application
	static const int chat_port;
	static const int data_port;

	// maximum messages in the send/receive queues
	static const int maximum_messages;

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
	CRITICAL_SECTION m_csControlSocketSend; // mutex for sending control messages

	// the control receive thread, receives messages through the network
	HANDLE m_hControlReceiveThread; // handle
	DWORD m_dwIDControlReceive; // thread id

	// the threads managing the control socket
	static DWORD ControlReceiveThread(NetworkManager* pNetworkManager);

	//-----------------------
	// Data Socket
	//-----------------------
	// the data socket, used for sending data, video, falcon, tactile ...
	NetworkSocket* m_pDataSocket; // the actual socket
	SOCKET m_hDataSocket; // the socket handle
	BOOL m_bDataConnected; // true if the connection has been accepted
	CRITICAL_SECTION m_csDataSocketSend; // mutex for sending data message

	// the control receive thread, receives messages through the network
	HANDLE m_hDataReceiveThread; // handle
	DWORD m_dwIDDataReceive; // thread id

	// the threads managing the data socket
	static DWORD DataReceiveThread(NetworkManager* pNetworkManager);

	//--------------------------
	// Private Member functions
	//--------------------------
	// closes the network sockets
	void closeSockets();

	// resets variables related to the connection status
	void resetConnection();

	// initialize the connection
	rc_network initializeConnection();

	// send a control message to the peer
	rc_network sendControlMessage(const ControlPacket& message);

	// send a data message to the peer
	rc_network asyncSendDataMessage(const DataPacket& message);

	// reset the network connection and notify the controller that the peer has disconnected
	void peerDisconnect();

	// reset the network connection and notify the controller that a network error has occured
	void networkError(rc_network error);

	// send a data message synchronously
	rc_network syncSendDataMessage(const DataPacket& packet);

	// handle a data message
	void handleDataMessage(const DataPacket& message);

	// send a control message synchronously
	rc_network syncSendControlMessage(const ControlPacket& packet);

	// handle a control message
	void handleControlMessage(const ControlPacket& message);

	//---------------------------
	// Private Data Members
	//---------------------------
	bool m_bIsConnected; // true if connected to a peer
	bool m_bIsServer; // true if we are the server (listener)
};

#endif // !defined(AFX_NETWORKMANAGER_H__3D85BBC3_3F80_477F_ABAB_1DAE8326532A__INCLUDED_)
