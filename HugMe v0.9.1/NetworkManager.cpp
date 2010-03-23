// NetworkManager.cpp: implementation of the NetworkManager class.
//
//////////////////////////////////////////////////////////////////////

// STL & Windows
#include "StdAfx.h"

#include "NetworkManager.h"
#include "SyncLocker.h"

const int NetworkManager::chat_port = 8869;
const int NetworkManager::data_port = 8870;
const int NetworkManager::maximum_messages = 10;
const int NetworkManager::maximum_control_packet_size = 4096;
const int NetworkManager::maximum_data_packet_size = 76800;

NetworkManager::NetworkManager() :
	// the control socket
	m_pControlSocket(NULL),
	m_hControlSocket(0),
	m_bControlConnected(false),
	m_sControlSocketSend(0, maximum_messages),
	m_sControlSocketReceive(0, maximum_messages),
	m_hControlSendThread(0),
	m_dwIDControlSend(0),
	m_hControlReceiveThread(0),
	m_dwIDControlReceive(0),
	m_hControlMessageHandleThread(0),
	m_dwIDControlMessageHandle(0),
	// the data socket
	m_pDataSocket(NULL),
	m_hDataSocket(0),
	m_bDataConnected(false),
	m_sDataSocketSend(0, maximum_messages),
	m_sDataSocketReceive(0, maximum_messages),
	m_hDataSendThread(0),
	m_dwIDDataSend(0),
	m_hDataReceiveThread(0),
	m_dwIDDataReceive(0),
	m_hDataMessageHandleThread(0),
	m_dwIDDataMessageHandle(0),
	// control members
	m_bIsConnected(false),
	m_bIsServer(false)
{
	InitializeCriticalSection(&m_csControlSocketSend);
	InitializeCriticalSection(&m_csControlSocketReceive);
	InitializeCriticalSection(&m_csDataSocketSend);
	InitializeCriticalSection(&m_csDataSocketReceive);
}

NetworkManager::~NetworkManager()
{
	DeleteCriticalSection(&m_csControlSocketSend);
	DeleteCriticalSection(&m_csControlSocketReceive);
	DeleteCriticalSection(&m_csDataSocketSend);
	DeleteCriticalSection(&m_csDataSocketReceive);
	delete m_pControlSocket;
	delete m_pDataSocket;
}

// start listening to for connections
rc_network NetworkManager::startListening()
{
	// don't try listening is we are already listening or connected
	if (m_pControlSocket != NULL)
	{
		if (m_bIsConnected)
		{
			return ERROR_ALREADY_CONNECTED;
		}
		else
		{
			return ERROR_ALREADY_LISTENING;
		}
	}

	// create the control socket
	m_pControlSocket = new NetworkSocket(this);
	if (!m_pControlSocket->Create(chat_port))
	{
		closeSockets();
		return ERROR_CREATE_CONTROL_SOCKET;
	}

	// listen on the control socket
	if (!m_pControlSocket->Listen())
	{
		closeSockets();
		return ERROR_LISTEN_CONTROL_SOCKET;
	}

	// create the data socket
	m_pDataSocket = new NetworkSocket(this);
	if (!m_pDataSocket->Create(data_port))
	{
		closeSockets();
		return ERROR_CREATE_DATA_SOCKET;
	}

	// listen on the data socket
	if (!m_pDataSocket->Listen())
	{
		closeSockets();
		return ERROR_LISTEN_DATA_SOCKET;
	}

	return SUCCESS;
}

rc_network NetworkManager::connect(const std::string& ipAddress)
{
	// don't try connecting if we are already connected
	if (m_pControlSocket != NULL)
	{
		if (isConnected())
		{
			return ERROR_ALREADY_CONNECTED;
		}
		else
		{
			return ERROR_ALREADY_LISTENING;
		}
	}

	// create the control socket
	m_pControlSocket = new NetworkSocket(this);
	if (!m_pControlSocket->Create())
	{
		closeSockets();
		return ERROR_CREATE_CONTROL_SOCKET;
	}

	// connect the control socket
	if (!m_pControlSocket->Connect(ipAddress.c_str(), chat_port))
	{
		closeSockets();
		return ERROR_CONNECT_CONTROL_SOCKET;
	}

	// create the data socket
	m_pDataSocket = new NetworkSocket(this);
	if (!m_pDataSocket->Create())
	{
		closeSockets();
		return ERROR_CREATE_DATA_SOCKET;
	}

	// connect the data socket
	if (!m_pDataSocket->Connect(ipAddress.c_str(), data_port))
	{
		closeSockets();
		return ERROR_CONNECT_DATA_SOCKET;
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

void NetworkManager::peerDisconnect()
{
	// close the sockets and reset the connection
	disconnect();

	// notify the controller that the peer has disconnected
	Controller::instance()->notifyPeerDisconnected();
}

void NetworkManager::networkError(rc_network error)
{
	// close the sockets and reset the connection
	disconnect();

	// notify the controller that the network has been disconnected due to an error
	Controller::instance()->notifyNetworkError(error);
}

void NetworkManager::notifyAccept(NetworkSocket* socket)
{
	// determine which socket has accepted a connection
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

	// when both sockets have accepted a connection we can initialize the connection
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

	return;
}

DWORD NetworkManager::ControlSendThread(NetworkManager *pNetworkManager)
{
	while (pNetworkManager->m_bIsConnected)
	{
		// wait for a message to arrive
		pNetworkManager->m_sControlSocketSend.Lock();

		// the packet to send
		ControlPacket packet;

		// need to synchronize access to the send queue,
		// another thread might try to add a message to the queue while we send
		synchronized (pNetworkManager->m_csControlSocketSend)
		{
			// get the packet at the front of the queue
			packet = pNetworkManager->m_ControlSocketSendQueue.front();

			// remove the packet from the queue of packets to send
			pNetworkManager->m_ControlSocketSendQueue.pop();
		}

		// get the data to send
		const std::vector<BYTE>& data = packet.getPacket();

		// send the message
		// When sending the message we might need to call the send function multiple times.
		// This is because it is not guaranteed to send all the bytes in one chunk.
		// We therefore have to keep calling the send function until all the bytes are sent.
		const char* i = (const char*)&data[0];
		int bytesSent = 0;
		while (bytesSent < data.size())
		{
			int rc = send(pNetworkManager->m_hControlSocket, i, data.size() - bytesSent, 0);

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
								pNetworkManager->networkError(ERROR_SOCKET_ERROR);
								return 0;
							}
							return 1;
						}
						break;
					}
				default: // some bytes have been sent, the return code represents how many
					bytesSent += rc;
					i += bytesSent;
			}
		}
	}
	return 1;
}

DWORD NetworkManager::ControlReceiveThread(NetworkManager* pNetworkManager)
{
	BYTE receivedBuffer[maximum_control_packet_size];
	std::vector<BYTE> queue;
	ControlPacket packet;
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
						pNetworkManager->networkError(ERROR_SOCKET_ERROR);
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

		// the while loop here may seem a little strange but it is needed
		// if the other sends 2 or more messages in quick succession then might read them both
		// in the same recv(...) call, we need to read as many messages as possible from the stream before continuing
		while (packet.readPacket(queue))
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
			packet = ControlPacket();
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
		ControlPacket packet;

		// we need to synchronize here so we don't remove a message from the queue while
		// the receive thread is adding a message
		synchronized(pNetworkManager->m_csControlSocketReceive)
		{
			// get the next message to handle
			packet = pNetworkManager->m_ControlSocketReceiveQueue.front();

			// remove the packet from the queue
			pNetworkManager->m_ControlSocketReceiveQueue.pop();
		}

		switch(packet.getType())
		{
			case CONTROL_PACKET_NAME:
			{
				// update the remote user name
				Controller::instance()->updateRemoteUserName(packet.getUserName());
				Controller::instance()->notifyNetworkConnectionAccepted();
				break;
			}
			case CONTROL_PACKET_CHAT:
			{
				// notify the controller that a new message has arrived
				Controller::instance()->notifyNewChatMessage(packet.getChatMessage());
				break;
			}
			case CONTROL_PACKET_START_GAME:
			{
				// notify the controller that the peer has started the game
				Controller::instance()->notifyPeerStartGame();
				break;
			}
			case CONTROL_PACKET_END_GAME:
			{
				// notify the controller that the peer has exited the game
				Controller::instance()->notifyPeerExitGame();
				break;
			}
			case CONTROL_PACKET_UNKNOWN:
			default:
			{
				// fail if we receive an unknown packet
				pNetworkManager->networkError(ERROR_UNKNOWN_CONTROL_MESSAGE);
				return 0;
			}
		}
	}
	return 1;
}

DWORD NetworkManager::DataSendThread(NetworkManager *pNetworkManager)
{
	// keep going until the network manager closes the connection
	while (pNetworkManager->m_bIsConnected)
	{
		// wait for a message to arrive
		pNetworkManager->m_sDataSocketSend.Lock();

		// the packet to send
		DataPacket packet;

		// need to synchronize access to the send queue,
		// another thread might try to add a message to the queue while we send
		synchronized (pNetworkManager->m_csDataSocketSend)
		{
			// get the packet at the front of the queue
			packet = pNetworkManager->m_DataSocketSendQueue.front();

			// remove the packet from the queue of packets to send
			pNetworkManager->m_DataSocketSendQueue.pop();
		}

		// get the assembled message in bytes
		std::auto_ptr<std::vector<BYTE> > ap_message = packet.getPacket();

		// the message to send in bytes
		std::vector<BYTE>& message = *ap_message;

		// send the message
		// When sending the message we might need to call the send function multiple times.
		// This is because it is not guaranteed to send all the bytes in one packet.
		// We therefore have to keep calling the send function until all the bytes are sent.
		const char* i = (const char*)&message[0];
		int bytesSent = 0;
		while (bytesSent < message.size())
		{
			int rc = send(pNetworkManager->m_hDataSocket, i, message.size() - bytesSent, 0);

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
								pNetworkManager->networkError(ERROR_SOCKET_ERROR);
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

DWORD NetworkManager::DataReceiveThread(NetworkManager* pNetworkManager)
{
	BYTE receivedBuffer[maximum_data_packet_size];
	std::vector<BYTE> queue;
	DataPacket packet = DataPacket();
	while (pNetworkManager->m_bIsConnected)
	{
		// sleep to prevent this thread from hogging the CPU
		Sleep(1);

		// receive data through the network
		int rc = recv(pNetworkManager->m_hDataSocket, (char*) receivedBuffer, maximum_control_packet_size, 0);

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
						pNetworkManager->networkError(ERROR_SOCKET_ERROR);
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

		// the while loop here may seem a little strange but it is needed
		// if the other sends 2 or more messages in quick succession then might read them both
		// in the same recv(...) call, we need to read as many messages as possible from the stream before continuing
		while (packet.readPacket(queue))
		{
			// the packet is complete, add it to the queue
			synchronized(pNetworkManager->m_csDataSocketReceive)
			{
				// add the packet to the end of the receive queue
				pNetworkManager->m_DataSocketReceiveQueue.push(packet);
			}

			// notify the message handler that a new packet has arrived
			pNetworkManager->m_sDataSocketReceive.Unlock();

			// start a new packet
			packet = DataPacket();
		}
	}
	return 1;
}

DWORD NetworkManager::DataMessageHandleThread(NetworkManager* pNetworkManager)
{
	// continue until we disconnect
	while(pNetworkManager->m_bIsConnected)
	{
		// wait for a message to arrive
		pNetworkManager->m_sDataSocketReceive.Lock();

		// the packet to handle
		DataPacket packet;

		// we need to synchronize here so we don't remove a message from the queue while
		// the receive thread is adding a message
		synchronized(pNetworkManager->m_csDataSocketReceive)
		{
			// get the next message to handle
			DataPacket packetToHandle = pNetworkManager->m_DataSocketReceiveQueue.front();

			// copy the packet over
			packet = DataPacket(packetToHandle);

			// remove the packet from the queue
			pNetworkManager->m_DataSocketReceiveQueue.pop();
		}

		if (packet.getHeader().isVideo)
		{
			Controller::instance()->notifyNewLocalVideoData(packet.getVideoRGBData());
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

	// initialize the socket threads
	// control socket threads
	m_hControlSendThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) ControlSendThread, (void*) this, 0, &m_dwIDControlSend);
	m_hControlReceiveThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) ControlReceiveThread, (void*) this, 0, &m_dwIDControlReceive);
	m_hControlMessageHandleThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) ControlMessageHandleThread, (void*) this, 0, &m_dwIDControlMessageHandle);

	// data socket threads
	m_hDataSendThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) DataSendThread, (void*) this, 0, &m_dwIDDataSend);
	m_hDataReceiveThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) DataReceiveThread, (void*) this, 0, &m_dwIDDataReceive);
	m_hDataMessageHandleThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) DataMessageHandleThread, (void*) this, 0, &m_dwIDDataMessageHandle);

	sendUserName(Controller::instance()->getLocalUserName());

	return SUCCESS;
}

rc_network NetworkManager::sendUserName(const std::string& userName)
{
	ControlPacket packet;
	packet.setUserName(userName);
	return sendControlMessage(packet);
}

rc_network NetworkManager::sendChatMessage(const std::string& message)
{
	ControlPacket packet;
	packet.setChatMessage(message);
	return sendControlMessage(packet);
}

rc_network NetworkManager::sendStartGame()
{
	ControlPacket packet;
	packet.setStartGame();
	return sendControlMessage(packet);
}

rc_network NetworkManager::sendEndGame()
{
	ControlPacket packet;
	packet.setEndGame();
	return sendControlMessage(packet);
}

rc_network NetworkManager::sendControlMessage(const ControlPacket& packet)
{
	// we can't send a message if we are not connected
	if (!isConnected())
	{
		return ERROR_NO_CONNECTION;
	}

	// we synchronize here to avoid adding a message to the queue
	// while a message is being sent
	synchronized(m_csControlSocketSend)
	{
		// add the message to the rear of the queue
		m_ControlSocketSendQueue.push(packet);
	}

	// notifies the send thread that there is a message to send
	if (!m_sControlSocketSend.Unlock())
	{
		return ERROR_MESSAGE_QUEUE_OVERFLOW;
	}
	return SUCCESS;
}

rc_network NetworkManager::sendDataMessage(const DataPacket& message)
{
	// we can't send a message if we are not connected
	if (!isConnected())
	{
		return ERROR_NO_CONNECTION;
	}

	// we synchronize here to avoid adding a message to the queue
	// while a message is being sent
	synchronized(m_csDataSocketSend)
	{
		// add the message to the rear of the queue
		m_DataSocketSendQueue.push(message);
	}

	// notifies the send thread that there is a message to send
	if (!m_sDataSocketSend.Unlock())
	{
		return ERROR_MESSAGE_QUEUE_OVERFLOW;
	}
	return SUCCESS;
}

rc_network NetworkManager::sendDataPacket(	const std::vector<BYTE>& vRGB,
											const std::vector<BYTE>& vDepth,
											const std::vector<BYTE>& vAR,
											const std::vector<BYTE>& vTactile)
{
	DataPacket message;
	message.setVideoData(vRGB, vDepth, vAR);
	message.setTactileData(vTactile);
	return sendDataMessage(message);
}

rc_network NetworkManager::sendVideoPacket(	const std::vector<BYTE>& vRGB,
											const std::vector<BYTE>& vDepth,
											const std::vector<BYTE>& vAR)
{
	DataPacket message;
	message.setVideoData(vRGB, vDepth, vAR);
	return sendDataMessage(message);
}

rc_network NetworkManager::sendTactilePacket(const std::vector<BYTE>& vTactile)
{
	DataPacket message;
	message.setTactileData(vTactile);
	return sendDataMessage(message);
}

bool NetworkManager::isConnected() const
{
	return m_bIsConnected;
}
