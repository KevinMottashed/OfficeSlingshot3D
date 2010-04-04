// NetworkManager.cpp: implementation of the NetworkManager class.
//
//////////////////////////////////////////////////////////////////////

// STL & Windows
#include "StdAfx.h"

#include "NetworkManager.h"
#include "SyncLocker.h"

// These are the constants for shutting down sockets for version 1 of the socket
// Version 2 of the socket has different constans (send and receive inverted)
// keep this in mind if we ever switch over
static const int shutdown_receive = 0;
static const int shutdown_send = 1;
static const int shutdown_receive_and_send = 2;

// various contants for ports and message sizes
const int NetworkManager::chat_port = 8869;
const int NetworkManager::data_port = 8870;
const int NetworkManager::maximum_control_packet_size = 4096;
const int NetworkManager::maximum_data_packet_size = 76800;

NetworkManager::NetworkManager() :
	// the control socket
	m_pControlSocket(NULL),
	m_hControlSocket(0),
	m_bControlConnected(false),
	m_hControlReceiveThread(0),
	m_dwIDControlReceive(0),
	// the data socket
	m_pDataSocket(NULL),
	m_hDataSocket(0),
	m_bDataConnected(false),
	m_hDataReceiveThread(0),
	m_dwIDDataReceive(0),
	// control members
	m_bIsConnected(false),
	m_bIsServer(false),
	m_bLocalDisconnect(false)
{
	InitializeCriticalSection(&m_csControlSocketSend);
	InitializeCriticalSection(&m_csDataSocketSend);
}

NetworkManager::~NetworkManager()
{
	DeleteCriticalSection(&m_csControlSocketSend);
	DeleteCriticalSection(&m_csDataSocketSend);
	delete m_pControlSocket;
	delete m_pDataSocket;
}

// start listening to for connections
rc_network NetworkManager::startListening()
{
	// start listening is an operation that modifies the connection status
	// we must therefore request write access to the connection resource before proceeding
	SyncWriterLock statusLock = SyncWriterLock(m_rwsync_ConnectionStatus);

	// don't try to connect if we are already connected
	if (m_bIsConnected)
	{
		return ERROR_ALREADY_CONNECTED;
	}

	// don't try listening if we are already listening
	if (m_pControlSocket != NULL)
	{
		return ERROR_ALREADY_LISTENING;
	}

	// create the control socket
	m_pControlSocket = new NetworkSocket(this);
	if (!m_pControlSocket->Create(chat_port))
	{
		resetSockets();
		return ERROR_CREATE_CONTROL_SOCKET;
	}

	// listen on the control socket
	if (!m_pControlSocket->Listen())
	{
		resetSockets();
		return ERROR_LISTEN_CONTROL_SOCKET;
	}

	// create the data socket
	m_pDataSocket = new NetworkSocket(this);
	if (!m_pDataSocket->Create(data_port))
	{
		resetSockets();
		return ERROR_CREATE_DATA_SOCKET;
	}

	// listen on the data socket
	if (!m_pDataSocket->Listen())
	{
		resetSockets();
		return ERROR_LISTEN_DATA_SOCKET;
	}

	return SUCCESS;
}

rc_network NetworkManager::connect(const std::string& ipAddress)
{
	{
		// connect is an operation that modifies the connection status
		// we must therefore request write access to the connection resource before proceeding
		SyncWriterLock statusLock = SyncWriterLock(m_rwsync_ConnectionStatus);

		// don't try connecting if we are already connected
		if (m_bIsConnected)
		{
			return ERROR_ALREADY_CONNECTED;
		}

		// don't try connecting if we are already listening
		if (m_pControlSocket != NULL)
		{
			return ERROR_ALREADY_LISTENING;
		}

		// create the control socket
		m_pControlSocket = new NetworkSocket(this);
		if (!m_pControlSocket->Create())
		{
			resetSockets();
			return ERROR_CREATE_CONTROL_SOCKET;
		}

		// connect the control socket
		if (!m_pControlSocket->Connect(ipAddress.c_str(), chat_port))
		{
			resetSockets();
			return ERROR_CONNECT_CONTROL_SOCKET;
		}

		// create the data socket
		m_pDataSocket = new NetworkSocket(this);
		if (!m_pDataSocket->Create())
		{
			resetSockets();
			return ERROR_CREATE_DATA_SOCKET;
		}

		// connect the data socket
		if (!m_pDataSocket->Connect(ipAddress.c_str(), data_port))
		{
			resetSockets();
			return ERROR_CONNECT_DATA_SOCKET;
		}

		// we are the client
		m_bIsServer = false;
		m_bIsConnected = true;
		initializeConnection();
	}

	// establish the connection (send user name)
	establishConnection();

	return SUCCESS;
}

rc_network NetworkManager::disconnect()
{
	// disconnect is an operation that modifies the connection status
	// we must therefore request write access to the connection resource before proceeding
	SyncWriterLock statusLock = SyncWriterLock(m_rwsync_ConnectionStatus);

	if (m_bIsConnected)
	{
		// it is important to only shutdown the sockets when we are already connected
		// if we close the sockets the other side will get a network error
		// it is only safe to close the sockets once both sides have shutdown the send operations
		// once the other side shuts down its sockets the receive threads will call the peer disconnect function
		// this will give us a clean disconnect
		shutdownSockets();

		// we set this variable so that the peerDisconnect call doesn't notify the UI that the peer has disconnected
		m_bLocalDisconnect = true;
	}
	else
	{
		// if we are not connected to anything then it is safe to close our sockets abruptly (no shutdown first)
		terminateConnection();
	}

	return SUCCESS;
}

void NetworkManager::peerDisconnect()
{
	// a peer disconnect is an operation that modifies the connection status
	// we must therefore request write access to the connection resource before proceeding
	SyncWriterLock statusLock = SyncWriterLock(m_rwsync_ConnectionStatus);

	if (m_bIsConnected && !m_bLocalDisconnect)
	{
		// notify the controller that the peer has disconnected
		// only notify the controller if it was the peer that initiated the disconnect
		Controller::instance()->notifyPeerDisconnected();
	}

	// close the sockets and reset the connection
	// we know that the peer has already shutdown it's send operations so it is safe to close the sockets
	terminateConnection();

	return;
}

void NetworkManager::networkError(rc_network error)
{
	// a network error is an operation that modifies the connection status
	// we must therefore request write access to the connection resource before proceeding
	SyncWriterLock statusLock = SyncWriterLock(m_rwsync_ConnectionStatus);

	if (m_bIsConnected)
	{
		// notify the controller that the network has been disconnected due to an error
		Controller::instance()->notifyNetworkError(error);
	}

	// close the sockets and reset the connection
	terminateConnection();

	return;
}

void NetworkManager::notifyAccept(NetworkSocket* socket)
{
	bool establish = false;

	// we must scope the write access because this function call will need read access to establish the connection
	// if a thread tries to get write and read access at the same time a deadlock occurs
	{
		// a network connection accept is an operation that modifies the connection status
		// we must therefore request write access to the connection resource before proceeding
		SyncWriterLock statusLock = SyncWriterLock(m_rwsync_ConnectionStatus);

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
			m_bIsConnected = true;
			initializeConnection();
			establish = true;
		}
	}

	if (establish)
	{
		establishConnection();
	}

	return;
}

void NetworkManager::resetConnectionStatus()
{
	// reset the connection status to not connected
	m_bControlConnected = false;
	m_bDataConnected = false;
	m_bIsConnected = false;
	m_bIsServer = false;
	m_bLocalDisconnect = false;

	return;
}

void NetworkManager::resetSockets()
{
	shutdownSockets();
	closeSockets();
}

void NetworkManager::shutdownSockets()
{
	// shutdown sockets
	if (m_hControlSocket)
	{
		// we only want to terminate the send so that the receive can terminate cleanly
		shutdown(m_hControlSocket, shutdown_send);
	}

	if (m_hDataSocket)
	{
		// we only want to terminate the send so that the receive can terminate cleanly
		shutdown(m_hDataSocket, shutdown_send);
	}

	return;
}

void NetworkManager::closeSockets()
{
	// Close sockets
	if (m_hControlSocket)
	{
		closesocket(m_hControlSocket);
		m_hControlSocket = 0;
	}

	if (m_hDataSocket)
	{
		closesocket(m_hDataSocket);
		m_hDataSocket = 0;
	}

	// free memory
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

DWORD NetworkManager::ControlReceiveThread(NetworkManager* pNetworkManager)
{
	BYTE receivedBuffer[maximum_control_packet_size];
	std::vector<BYTE> queue;
	ControlPacket packet;

	// we need to use flags for operations that require write access to the connection status
	// this thread will use read access to receive data so when a disconnect or error occurs
	// we will release the read access and obtain write access to react to the disconnect/error
	// we do this because a deadlock occurs if a thread tries to obtain both read an write access
	bool peerDisconnect = false;
	bool socketError = false;

	// keep going until the network connection has been terminated,
	// a peer disconnect occurs or a network error occurs
	while (true)
	{
		// look for operations that require write access
		if (peerDisconnect)
		{
			pNetworkManager->peerDisconnect();
			return 0;
		}
		else if (socketError)
		{
			pNetworkManager->networkError(ERROR_SOCKET_ERROR);
			return 0;
		}

		// receiving data through the control socket is an operation that reads the connection status
		// we must therefore request read access to the connection resource before proceeding
		SyncReaderLock statusLock = SyncReaderLock(pNetworkManager->m_rwsync_ConnectionStatus);

		if (!pNetworkManager->m_bIsConnected)
		{
			// we are no longer connected, we can stop receiving data
			return 0;
		}

		// sleep to prevent this thread from hogging the CPU
		Sleep(1);

		// receive data through the network
		int rc = recv(pNetworkManager->m_hControlSocket, (char*) receivedBuffer, maximum_control_packet_size, 0);

		switch (rc) {
		case SOCKET_ERROR: // fail when a socket error occurs
			{
				DWORD error = WSAGetLastError();
				// ignore the would block error, it means the socket is temporarily unavailable
				if (error != WSAEWOULDBLOCK)
				{
					// disconnect when the socket fails
					socketError = true;
				}
				// this read failed, we don't want to try to handle an empty message so try reading again
				continue;
			}
		case 0:
			// recv returns 0 to indicate an end of stream
			// this means the peer has disconnected
			peerDisconnect = true;
			continue;
		}

		// add the received bytes to the end of the queue
		queue.insert(queue.end(), &receivedBuffer[0], &receivedBuffer[rc]);

		// the while loop here may seem a little strange but it is needed
		// if the other sends 2 or more messages in quick succession then might read them both
		// in the same recv(...) call, we need to read as many messages as possible from the stream before continuing
		while (packet.readPacket(queue))
		{
			pNetworkManager->handleControlMessage(packet);

			// start a new packet
			packet = ControlPacket();
		}
	}
	return 1;
}

DWORD NetworkManager::DataReceiveThread(NetworkManager* pNetworkManager)
{
	BYTE receivedBuffer[maximum_data_packet_size];
	std::vector<BYTE> queue;
	DataPacket packet = DataPacket();

	// we need to use flags for operations that require write access to the connection status
	// this thread will use read access to receive data so when a disconnect or error occurs
	// we will release the read access and obtain write access to react to the disconnect/error
	// we do this because a deadlock occurs if a thread tries to obtain both read an write access
	bool peerDisconnect = false;
	bool socketError = false;

	// keep going until the network connection has been terminated,
	// a peer disconnect occurs or a network error occurs
	while (true)
	{
		// look for operations that require write access
		if (peerDisconnect)
		{
			pNetworkManager->peerDisconnect();
			return 0;
		}
		else if (socketError)
		{
			pNetworkManager->networkError(ERROR_SOCKET_ERROR);
			return 0;
		}

		// receiving data through the data socket is an operation that reads the connection status
		// we must therefore request read access to the connection resource before proceeding
		SyncReaderLock statusLock = SyncReaderLock(pNetworkManager->m_rwsync_ConnectionStatus);

		if (!pNetworkManager->m_bIsConnected)
		{
			// we are no longer connected, we can stop receiving data
			return 0;
		}

		// receive data through the network
		int rc = recv(pNetworkManager->m_hDataSocket, (char*) receivedBuffer, maximum_control_packet_size, 0);

		switch (rc) {
		case SOCKET_ERROR: // fail when a socket error occurs
			{
				DWORD error = WSAGetLastError();
				// ignore the would block error, it means the socket is temporarily unavailable
				if (error != WSAEWOULDBLOCK)
				{
					// disconnect when the socket fails
					socketError = true;
				}

				// this read failed, we don't want to try to handle an empty message so try reading again
				continue;
			}
		case 0:
			// recv returns 0 to indicate an end of stream
			// this means the peer has disconnected
			peerDisconnect = true;
			continue;
		}

		// add the received bytes to the end of the queue
		queue.insert(queue.end(), &receivedBuffer[0], &receivedBuffer[rc]);

		// the while loop here may seem a little strange but it is needed
		// if the other sends 2 or more messages in quick succession then might read them both
		// in the same recv(...) call, we need to read as many messages as possible from the stream before continuing
		while (packet.readPacket(queue))
		{
			// notify the message handler that a new packet has arrived
			pNetworkManager->handleDataMessage(packet);

			// start a new packet
			packet = DataPacket();
		}
	}
	return 1;
}

rc_network NetworkManager::initializeConnection()
{
	// detach the sockets from their threads
	m_hControlSocket = m_pControlSocket->Detach();
	m_hDataSocket = m_pDataSocket->Detach();

	// initialize the socket threads
	// control socket threads
	m_hControlReceiveThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) ControlReceiveThread, (void*) this, 0, &m_dwIDControlReceive);

	// data socket threads
	m_hDataReceiveThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) DataReceiveThread, (void*) this, 0, &m_dwIDDataReceive);

	return SUCCESS;
}

rc_network NetworkManager::establishConnection()
{
	// we only need to send our username to establish the connection
	return sendUserName(Controller::instance()->getLocalUserName());
}

void NetworkManager::terminateConnection()
{
	// close the sockets and reset the connection
	resetConnectionStatus();
	resetSockets();
	return;
}

rc_network NetworkManager::sendUserName(const std::string& userName)
{
	ControlPacket packet;
	packet.setUserName(userName);
	return syncSendControlMessage(packet);
}

rc_network NetworkManager::sendChatMessage(const std::string& message)
{
	ControlPacket packet;
	packet.setChatMessage(message);
	return syncSendControlMessage(packet);
}

rc_network NetworkManager::sendStartGame()
{
	ControlPacket packet;
	packet.setStartGame();
	return syncSendControlMessage(packet);
}

rc_network NetworkManager::sendEndGame()
{
	ControlPacket packet;
	packet.setEndGame();
	return syncSendControlMessage(packet);
}

rc_network NetworkManager::sendVideoData(const char* pVideoData, unsigned int size)
{
	DataPacket message;
	message.setVideoData(pVideoData, size);
	return syncSendDataMessage(message);
}

rc_network NetworkManager::sendPlayerPosition(const cVector3d& position)
{
	DataPacket message;
	message.setPlayerPosition(position);
	return syncSendDataMessage(message);
}

rc_network NetworkManager::sendSlingshotPosition(const cVector3d& position)
{
	DataPacket message;
	message.setSlingshotPosition(position);
	return syncSendDataMessage(message);
}

rc_network NetworkManager::sendProjectile(const Projectile& projectile)
{
	DataPacket message;
	message.setProjectile(projectile);
	return syncSendDataMessage(message);
}

rc_network NetworkManager::sendSlingshotPullback()
{
	DataPacket message;
	message.setSlingshotPullback();
	return syncSendDataMessage(message);
}


rc_network NetworkManager::syncSendDataMessage(const DataPacket& packet)
{
	// sending data through the control socket is an operation that reads the connection status
	// we must therefore request read access to the connection resource before proceeding
	SyncReaderLock statusLock = SyncReaderLock(m_rwsync_ConnectionStatus);

	if (!m_bIsConnected)
	{
		return ERROR_NO_CONNECTION;
	}

	// the message to send in bytes
	const std::vector<BYTE>& message = packet.getPacket();

	// send the message
	// When sending the message we might need to call the send function multiple times.
	// This is because it is not guaranteed to send all the bytes in one packet.
	// We therefore have to keep calling the send function until all the bytes are sent.
	const char* i = (const char*)&message[0];
	unsigned int bytesSent = 0;

	// we need to synchronize here so that only one thread is sending data on the socket at a time
	SyncLocker sendLock = SyncLocker(m_csDataSocketSend);

	while (bytesSent < message.size())
	{
		int rc = send(m_hDataSocket, i, message.size() - bytesSent, 0);

		// check the return code
		switch (rc)
		{
			case 0: // no bytes have been sent, consider this a failure
			{
				return ERROR_SOCKET_ERROR;
			}
			case SOCKET_ERROR: // there's a problem with the socket, fail here
			{
				DWORD error = WSAGetLastError();
				// ignore the would block error, it means the socket is temporarily unavailable
				if (error != WSAEWOULDBLOCK)
				{
					// fail when a socket error occurs
					return ERROR_SOCKET_ERROR;
				}
				break;
			}
			default: // some bytes have been sent, the return code represent how many
			{
				bytesSent += rc;
				i += bytesSent;
			}
		}
	}
	return SUCCESS;
}

void NetworkManager::handleDataMessage(const DataPacket& message)
{
	switch (message.getType())
	{
		case DATA_PACKET_VIDEO:
		{
			Controller::instance()->notifyNewRemoteVideoData(message.getVideoData(), message.getVideoDataSize());
			break;
		}
		case DATA_PACKET_PLAYER_POSITION:
		{
			Controller::instance()->notifyNewRemotePlayerPosition(message.getPlayerPosition());
			break;
		}
		case DATA_PACKET_SLINGSHOT_POSITION:
		{
			Controller::instance()->notifyNewRemoteSlingshotPosition(message.getSlingshotPosition());
			break;
		}
		case DATA_PACKET_PROJECTILE:
		{
			Controller::instance()->notifyNewRemoteProjectile(message.getProjectile());
			break;
		}
		case DATA_PACKET_SLINGSHOT_PULLBACK:
		{
			Controller::instance()->notifyRemoteSlingshotPullback();
			break;
		}
		case DATA_PACKET_UNKNOWN:
		default:
		{
			// ignore unknown packets
			break;
		}
	}
	return;
}

rc_network NetworkManager::syncSendControlMessage(const ControlPacket& packet)
{
	// sending data through the control socket is an operation that reads the connection status
	// we must therefore request read access to the connection resource before proceeding
	SyncReaderLock statusLock = SyncReaderLock(m_rwsync_ConnectionStatus);

	if (!m_bIsConnected)
	{
		return ERROR_NO_CONNECTION;
	}

	// get the data to send
	const std::vector<BYTE>& data = packet.getPacket();

	// send the message
	// When sending the message we might need to call the send function multiple times.
	// This is because it is not guaranteed to send all the bytes in one chunk.
	// We therefore have to keep calling the send function until all the bytes are sent.
	const char* i = (const char*)&data[0];
	unsigned int bytesSent = 0;

	// we need to synchronize here so that only one thread is sending data on the socket at a time
	SyncLocker sendLock = SyncLocker(m_csControlSocketSend);

	while (bytesSent < data.size())
	{
		int rc = send(m_hControlSocket, i, data.size() - bytesSent, 0);

		// check the return code
		switch (rc)
		{
			case 0: // no bytes have been sent, consider this a failure
				return ERROR_SOCKET_ERROR;
			case SOCKET_ERROR: // there's a problem with the socket, fail here
				{
					DWORD error = WSAGetLastError();
					// ignore the would block error, it means the socket is temporarily unavailable
					if (error != WSAEWOULDBLOCK)
					{
						// fail when a socket error occurs
						return ERROR_SOCKET_ERROR;
					}
					break;
				}
			default: // some bytes have been sent, the return code represents how many
				bytesSent += rc;
				i += bytesSent;
		}
	}
	return SUCCESS;
}

void NetworkManager::handleControlMessage(const ControlPacket& message)
{
	switch(message.getType())
	{
		case CONTROL_PACKET_NAME:
		{
			// update the remote user name
			Controller::instance()->updateRemoteUserName(message.getUserName());
			Controller::instance()->notifyNetworkConnectionAccepted();
			break;
		}
		case CONTROL_PACKET_CHAT:
		{
			// notify the controller that a new message has arrived
			Controller::instance()->notifyNewChatMessage(message.getChatMessage());
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
			// ignore unknown packets
			break;
		}
	}
	return;
}
