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
	NetworkManager(Controller* pController);
	virtual ~NetworkManager();

	// start listening for connections
	rc_network startListening();

	// connect to a host
	rc_network connect(const CString& ipAdrress);

	// disconnect from a host
	rc_network disconnect();

	// return true if we are connected to a peer
	bool isConnected() const;

	// send the player's user name over the network
	rc_network sendUserName(const std::string& userName);

	// send a chat message to the other player
	rc_network sendChatMessage(const std::string& message);

	// send a full data packet to the other player, video + tactile
	rc_network sendDataPacket(	const std::vector<BYTE>& vRGB,
								const std::vector<BYTE>& vDepth,
								const std::vector<BYTE>& vAR,
								const std::vector<BYTE>& vTactile);

	// send a video only packet to the other player
	rc_network sendVideoPacket(	const std::vector<BYTE>& vRGB,
								const std::vector<BYTE>& vDepth,
								const std::vector<BYTE>& vAR);

	// send a tactile only packet to the other player
	rc_network sendTactilePacket(const std::vector<BYTE>& vTactile);

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

	std::queue<ControlPacket> m_ControlSocketSendQueue; // send queue
	CSemaphore m_sControlSocketSend; // semaphore for how many messages in the send queue
	CRITICAL_SECTION m_csControlSocketSend; // critical section for the send queue

	std::queue<ControlPacket> m_ControlSocketReceiveQueue; // receive queue
	CSemaphore m_sControlSocketReceive; // semaphore for how many messages in the receive queue
	CRITICAL_SECTION m_csControlSocketReceive; // critical section for the receive queue

	// the control send thread, sends messages through the network
	HANDLE m_hControlSendThread; // handle
	DWORD m_dwIDControlSend; // thread id

	// the control receive thread, receives messages through the network
	HANDLE m_hControlReceiveThread; // handle
	DWORD m_dwIDControlReceive; // thread id

	// the control message handle thread, handles messages received through the network
	HANDLE m_hControlMessageHandleThread; // handle
	DWORD m_dwIDControlMessageHandle; // thread id

	// the threads managing the control socket
	static DWORD ControlSendThread(NetworkManager* pNetworkManager);
	static DWORD ControlReceiveThread(NetworkManager* pNetworkManager);
	static DWORD ControlMessageHandleThread(NetworkManager* pNetworkManager);

	//-----------------------
	// Data Socket
	//-----------------------
	// the data socket, used for sending data, video, falcon, tactile ...
	NetworkSocket* m_pDataSocket; // the actual socket
	SOCKET m_hDataSocket; // the socket handle
	BOOL m_bDataConnected; // true if the connection has been accepted

	std::queue<DataPacket> m_DataSocketSendQueue; // send queue
	CSemaphore m_sDataSocketSend; // semaphore for how many messages in the send queue
	CRITICAL_SECTION m_csDataSocketSend; // critical section for the send queue

	std::queue<DataPacket> m_DataSocketReceiveQueue; // receive queue
	CSemaphore m_sDataSocketReceive; // semaphore for how many messages in the receive queue
	CRITICAL_SECTION m_csDataSocketReceive; // critical section for the receive queue

	// the data game thread, sends messages through the network
	HANDLE m_hDataSendThread; // handle
	DWORD m_dwIDDataSend; // thread id

	// the control receive thread, receives messages through the network
	HANDLE m_hDataReceiveThread; // handle
	DWORD m_dwIDDataReceive; // thread id

	// the control message handle thread, handles messages received through the network
	HANDLE m_hDataMessageHandleThread; // handle
	DWORD m_dwIDDataMessageHandle; // thread id

	// the threads managing the data socket
	static DWORD DataSendThread(NetworkManager* pNetworkManager);
	static DWORD DataReceiveThread(NetworkManager* pNetworkManager);
	static DWORD DataMessageHandleThread(NetworkManager* pNetworkManager);

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
	rc_network sendDataMessage(const DataPacket& message);

	// reset the network connection and notify the controller that the peer has disconnected
	rc_network peerDisconnect();

	// reset the network connection and notify the controller that a network error has occured
	rc_network networkError();

	//---------------------------
	// Private Data Members
	//---------------------------
	Controller* m_pController; // our controller
	bool m_bIsConnected; // true if connected to a peer
	bool m_bIsServer; // true if we are the server (listener)
};

#endif // !defined(AFX_NETWORKMANAGER_H__3D85BBC3_3F80_477F_ABAB_1DAE8326532A__INCLUDED_)
