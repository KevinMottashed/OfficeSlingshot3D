// NetworkManager.h: interface for the NetworkManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NETWORKMANAGER_H__3D85BBC3_3F80_477F_ABAB_1DAE8326532A__INCLUDED_)
#define AFX_NETWORKMANAGER_H__3D85BBC3_3F80_477F_ABAB_1DAE8326532A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// This is microsft's way of dealing with this warning in vc++6
// the warning occurs because the STL is used
#pragma warning (disable : 4786)

// STL
#include <string>
#include <queue>
#include <vector>

// Windows
#include <afxmt.h> // for semaphore

#include "Controller.h"
#include "NetworkSocket.h"
#include "NetworkCodes.h"
#include "ChatPacket.h"

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

	// send the player's user name over the network
	rc_network sendUserName(const CString& userName);

	// notify the controller that a network connection has been accepted
	void notifyAccept(NetworkSocket* socket);

private:
	// the network ports for our application
	static const int chat_port;
	static const int data_port;

	// maximum messages in the send/receive queues
	static const int maximum_messages;

	// maximum size for a control packet
	static const int maximum_control_packet_size;

	// the control socket, used for sending chat messages, user names, game data ...
	NetworkSocket* m_pControlSocket; // the actual socket
	SOCKET m_hControlSocket; // the socket handle
	BOOL m_bControlConnected; // true if the connection has been accepted

	std::queue<CChatPacket> m_ControlSocketSendQueue; // send queue
	CSemaphore m_sControlSocketSend; // semaphore for how many messages in the send queue
	CRITICAL_SECTION m_csControlSocketSend; // critical section for the send queue

	std::queue<CChatPacket> m_ControlSocketReceiveQueue; // receive queue
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


	static DWORD ControlSendThread(NetworkManager* pNetworkManager);
	static DWORD ControlReceiveThread(NetworkManager* pNetworkManager);
	static DWORD ControlMessageHandleThread(NetworkManager* pNetworkManager);

	// the data socket, used for sending data, video, falcon, tactile ...
	NetworkSocket* m_pDataSocket; // the actual socket
	SOCKET m_hDataSocket; // the socket handle
	BOOL m_bDataConnected; // true if the connection has been accepted
	std::queue<std::vector<BYTE> > m_DataSocketSendQueue; // send queue
	std::queue<std::vector<BYTE> > m_DataSocketReceiveQueue; // receive queue

	void closeSockets();

	// initialize the connection
	rc_network initializeConnection();

	// send a control message to the peer
	rc_network sendControlMessage(const CChatPacket& message);

	Controller* m_pController;
	bool m_bIsConnected;
	bool m_bIsServer;
	bool m_bUserNameReceived;
	bool m_bConfigurationReceived;
};

#endif // !defined(AFX_NETWORKMANAGER_H__3D85BBC3_3F80_477F_ABAB_1DAE8326532A__INCLUDED_)
