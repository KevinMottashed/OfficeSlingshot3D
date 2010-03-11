// NetworkManager.cpp: implementation of the NetworkManager class.
//
//////////////////////////////////////////////////////////////////////

// STL
#include <algorithm>

// Windows
#include "stdafx.h"

#include "NetworkManager.h"
#include "Sync.h"

const int NetworkManager::chat_port = 8869;
const int NetworkManager::data_port = 8870;
const int NetworkManager::maximum_messages = 10;
const int NetworkManager::maximum_control_packet_size = 4096;

NetworkManager::NetworkManager(Controller* controller) :
	m_pControlSocket(NULL),
	m_hControlSocket(0),
	m_bControlConnected(false),
	m_pDataSocket(NULL),
	m_hDataSocket(0),
	m_bDataConnected(false),
	m_pController(controller),
	m_sControlSocketSend(0, maximum_messages),
	m_sControlSocketReceive(0, maximum_messages),
	m_bUserNameReceived(false),
	m_bConfigurationReceived(false)
{
	InitializeCriticalSection(&m_csControlSocketSend);
	InitializeCriticalSection(&m_csControlSocketReceive);
}

NetworkManager::~NetworkManager()
{
	DeleteCriticalSection(&m_csControlSocketSend);
	DeleteCriticalSection(&m_csControlSocketReceive);
	delete m_pControlSocket;
	delete m_pDataSocket;
}

// start listening to for connections
rc_network NetworkManager::startListening()
{
	if (m_pControlSocket != NULL)
	{
		return ERROR_ALREADY_LISTENING;
	}

	m_pControlSocket = new NetworkSocket(this);
	if (!m_pControlSocket->Create(chat_port))
	{
		closeSockets();
		return ERROR_CREATE_SOCKET;
	}

	if (!m_pControlSocket->Listen())
	{
		closeSockets();
		return ERROR_LISTEN_SOCKET;
	}

	m_pDataSocket = new NetworkSocket(this);
	if (!m_pDataSocket->Create(data_port))
	{
		closeSockets();
		return ERROR_CREATE_SOCKET;
	}

	if (!m_pDataSocket->Listen())
	{
		closeSockets();
		return ERROR_LISTEN_SOCKET;
	}

	return SUCCESS;
}

rc_network NetworkManager::connect(const CString& ipAddress)
{
	if (m_pControlSocket != NULL)
	{
		return ERROR_ALREADY_CONNECTED;
	}

	// connect the control socket
	m_pControlSocket = new NetworkSocket(this);
	if (!m_pControlSocket->Create())
	{
		closeSockets();
		return ERROR_CREATE_SOCKET;
	}

	if (!m_pControlSocket->Connect(ipAddress, chat_port))
	{
		closeSockets();
		return ERROR_CONNECT;
	}

	// connect the data socket
	m_pDataSocket = new NetworkSocket(this);
	if (!m_pDataSocket->Create())
	{
		closeSockets();
		return ERROR_CREATE_SOCKET;
	}

	if (!m_pDataSocket->Connect(ipAddress, data_port))
	{
		closeSockets();
		return ERROR_CONNECT;
	}

	// we are the client
	m_bIsServer = false;
	initializeConnection();

	return SUCCESS;
}

rc_network NetworkManager::disconnect()
{
	// close the sockets and reset the connection
	closeSockets();
	resetConnection();
	return SUCCESS;
}

rc_network NetworkManager::peerDisconnect()
{
	// close the sockets and reset the connection
	disconnect();

	// notify the controller that the peer has disconnected
	m_pController->notifyPeerDisconnected();

	return SUCCESS;
}

rc_network NetworkManager::networkError()
{
	// close the sockets and reset the connection
	disconnect();

	// notify the controller that the network has been disconnected due to an error
	m_pController->notifyNetworkError();

	return SUCCESS;
}

rc_network NetworkManager::startVideo()
{
	// **** Implement disconnect ****

	return SUCCESS;
}

rc_network NetworkManager::exitVideo()
{
	// **** Implement disconnect ****

	return SUCCESS;
}


void NetworkManager::notifyAccept(NetworkSocket* socket)
{
	if (socket == m_pControlSocket)
	{
		m_bControlConnected = true;
	}
	else if (socket == m_pDataSocket)
	{
		m_bDataConnected = true;
	}
	else
	{
		return;
	}

	if (m_bControlConnected && m_bDataConnected)
	{
		// we are the server
		m_bIsServer = true;
		initializeConnection();
	}
	return;
}

void NetworkManager::resetConnection()
{
	// reset the connection status to not connected
	m_bControlConnected = false;
	m_bDataConnected = false;
	m_bIsConnected = false;
	m_bIsServer = false;

	// empty the control receive queue in a thread safe manner
	while (m_sControlSocketReceive.Lock(1)) // wait 1 ms for the lock
	{
		synchronized(m_csControlSocketReceive)
		{
			m_ControlSocketReceiveQueue.pop();
		}
	}

	// empty the control send queue in a thread safe manner
	while (m_sControlSocketSend.Lock(1)) // wait 1 ms for the lock
	{
		synchronized(m_csControlSocketSend)
		{
			m_ControlSocketSendQueue.pop();
		}
	}

	return;
}

void NetworkManager::closeSockets()
{
	// Close sockets
	if (m_hControlSocket)
	{
		// 0 represents shutdown the socket's send functionality
		// there is a constant for this but including that header causes many problems
		// we only want to terminate the send so that the receive can terminate cleanly
		shutdown(m_hControlSocket, 0);
		closesocket(m_hControlSocket);
		m_hControlSocket = 0;
	}

	if (m_hDataSocket)
	{
		// 0 represents shutdown the socket's send functionality
		// there is a constant for this but including that header causes many problems
		// we only want to terminate the send so that the receive can terminate cleanly
		shutdown(m_hDataSocket, 0);
		closesocket(m_hDataSocket);
		m_hDataSocket = 0;
	}

	if (m_pControlSocket)
	{
		delete m_pControlSocket;
		m_pControlSocket = NULL;
	}

	if (m_pDataSocket)
	{
		delete m_pDataSocket;
		m_pDataSocket = NULL;
	}
}

DWORD NetworkManager::ControlSendThread(NetworkManager *pNetworkManager)
{
	// keep going until the network manager closes the connection
	while (pNetworkManager->m_bIsConnected)
	{
		// wait for a message to arrive
		pNetworkManager->m_sControlSocketSend.Lock();

		// the packet to send
		CChatPacket packet;

		// need to synchronize access to the send queue,
		// another thread might try to add a message to the queue while we send
		synchronized (pNetworkManager->m_csControlSocketSend)
		{
			// get the packet at the front of the queue
			CChatPacket packetToSend = pNetworkManager->m_ControlSocketSendQueue.front();

			// copy it into our buffer
			packet = CChatPacket(packetToSend);

			// remove the packet from the queue of packets to send
			pNetworkManager->m_ControlSocketSendQueue.pop();
		}

		// the message to send in bytes
		std::vector<BYTE> message;

		// packet format is <size><type><data>
		unsigned int size = sizeof(unsigned int) + sizeof(CChatPacket::PacketType) + packet.getPacketSize();
		CChatPacket::PacketType type = packet.getType();

		// add the total size to the packet
		message.insert(message.end(), (BYTE*) &size, ((BYTE*) (&size)) + sizeof(unsigned int));

		// add the type to the packet
		message.insert(message.end(), (BYTE*) &type, ((BYTE*) (&type)) + sizeof(CChatPacket::PacketType));

		// add the data to the packet
		message.insert(message.end(), packet.getPacketPtr(), packet.getPacketPtr() + packet.getPacketSize());

		// send the message
		// When sending the message we might need to call the send function multiple times.
		// This is because it is not guaranteed to send all the bytes in one packet.
		// We therefore have to keep calling the send function until all the bytes are sent.
		const char* i = (const char*)&message[0];
		int bytesSent = 0;		
		while (bytesSent < message.size())
		{
			int rc = send(pNetworkManager->m_hControlSocket, i, message.size() - bytesSent, 0);

			// check the return code
			switch (rc)
			{
				case 0: // no bytes have been sent, consider this a failure
					return 0;
				case SOCKET_ERROR: // there's a problem with the socket, fail here
					{
						DWORD error = GetLastError();
						// ignore the would block error, it means the socket is temporarily unavailable
						if (error != WSAEWOULDBLOCK)
						{
							// the error is only genuine if we are still connected
							// when the network is disconnected by the user we will receive a socket error
							// this is normal, so do not notify the controller
							if (pNetworkManager->m_bIsConnected)
							{
								// disconnect when the socket fails
								pNetworkManager->networkError();
								return 0;
							}
							return 1;
						}
						break;
					}
				default: // some bytes have been sent, the return code represent how many
					bytesSent += rc;
					i += bytesSent;
			}
		}
	}
	// exited gracefully
	return 1;
}

DWORD NetworkManager::ControlReceiveThread(NetworkManager* pNetworkManager)
{
	BYTE receivedBuffer[maximum_control_packet_size];
	vector<BYTE> queue;
	CChatPacket packet = CChatPacket();
	while (pNetworkManager->m_bIsConnected)
	{
		// sleep to prevent this thread from hogging the CPU
		Sleep(1);

		// receive data through the network
		int rc = recv(pNetworkManager->m_hControlSocket, (char*) receivedBuffer, maximum_control_packet_size, 0);

		switch (rc) {
		case SOCKET_ERROR: // fail when a socket error occurs
			{	
				DWORD error = GetLastError();
				// ignore the would block error, it means the socket is temporarily unavailable
				if (error != WSAEWOULDBLOCK)
				{
					// the error is only genuine if we are still connected
					// when the network is disconnected by the user we will receive a socket error
					// this is normal, so do not notify the controller
					if (pNetworkManager->m_bIsConnected)
					{
						// disconnect when the socket fails
						pNetworkManager->networkError();
						return 0;
					}
					return 1;
				}
				// this read failed, we don't want to try to handle an empty message so try reading again
				continue;
			}

		case 0:
			// recv returns 0 to indicate an end of stream
			// this means the peer has disconnected
			pNetworkManager->peerDisconnect();
			return 0;
		}

		// add the received bytes to the end of the queue
		queue.insert(queue.end(), &receivedBuffer[0], &receivedBuffer[rc]);

		if (packet.readPacket(queue))
		{
			// the packet is complete, add it to the queue
			synchronized(pNetworkManager->m_csControlSocketReceive)
			{
				// add the packet to the end of the receive queue
				pNetworkManager->m_ControlSocketReceiveQueue.push(packet);
			}

			// notify the message handler that a new packet has arrived
			pNetworkManager->m_sControlSocketReceive.Unlock();

			// start a new packet
			packet = CChatPacket();
		}
	}
	return 1;
}

DWORD NetworkManager::ControlMessageHandleThread(NetworkManager* pNetworkManager)
{
	// continue until we disconnect
	while(pNetworkManager->m_bIsConnected)
	{
		// wait for a message to arrive
		pNetworkManager->m_sControlSocketReceive.Lock();

		// the packet to handle
		CChatPacket packet;

		// we need to synchronize here so we don't remove a message from the queue while
		// the receive thread is adding a message
		synchronized(pNetworkManager->m_csControlSocketReceive)
		{
			// get the next message to handle
			CChatPacket packetToHandle = pNetworkManager->m_ControlSocketReceiveQueue.front();

			// copy the packet over
			packet = CChatPacket(packetToHandle);

			// remove the packet from the queue
			pNetworkManager->m_ControlSocketReceiveQueue.pop();
		}

		switch (packet.getType())
		{
		case CChatPacket::PACKET_DISCONNET:
			// TODO not implemented yet
			break;
		case CChatPacket::PACKET_CHAT:
			// TODO not implemented yet
			break;
		case CChatPacket::PACKET_NAME:
		{
			packet.writeChar('\0'); // make sure the name is null terminated
			std::string remoteUserName = (LPCSTR) packet.getPacketPtr();
			pNetworkManager->m_pController->updateRemoteUserName(remoteUserName);
			pNetworkManager->m_bUserNameReceived = true;
			if (pNetworkManager->m_bUserNameReceived) // && m_bConfigurationReceived
			{
				pNetworkManager->m_pController->notifyNetworkConnectionAccepted();
			}
			break;
		}
		case CChatPacket::PACKET_UDP_PORT:
			// TODO not implemented yet
			break;
		case CChatPacket::PACKET_CONFIG:
			// TODO not implemented yet
			break;
		case CChatPacket::PACKET_FORCE_DWORD:
			// TODO not implemented yet
			break;
		default:
			return 0; // unknown packet type
		}
	}
	return 1;
}

rc_network NetworkManager::initializeConnection()
{
	// the connection has been established
	m_bIsConnected = true;

	// detach the sockets from their threads
	m_hControlSocket = m_pControlSocket->Detach();
	m_hDataSocket = m_pDataSocket->Detach();

	// initialize the threads
	m_hControlSendThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) ControlSendThread, (void*) this, 0, &m_dwIDControlSend);
	m_hControlReceiveThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) ControlReceiveThread, (void*) this, 0, &m_dwIDControlReceive);
	m_hControlMessageHandleThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) ControlMessageHandleThread, (void*) this, 0, &m_dwIDControlMessageHandle);

	// send our user name to the peer
	if (m_bIsServer)
	{
		sendUserName("rainbow");
	}
	else
	{
		sendUserName("sunshine");
	}

	return SUCCESS;
}

rc_network NetworkManager::sendUserName(const CString& userName)
{
	CChatPacket userNamePacket;
	userNamePacket.setType(CChatPacket::PACKET_NAME);
	userNamePacket.writeString(userName);
	return sendControlMessage(userNamePacket);
}

rc_network NetworkManager::sendControlMessage(const CChatPacket& message)
{
	// we synchronize here to avoid adding a message to the queue
	// while a message is being sent
	synchronized(m_csControlSocketSend)
	{
		// add the message to the rear of the queue
		m_ControlSocketSendQueue.push(message);
	}

	// notifies the send thread that there is a message to send
	m_sControlSocketSend.Unlock();

	return SUCCESS;
}

