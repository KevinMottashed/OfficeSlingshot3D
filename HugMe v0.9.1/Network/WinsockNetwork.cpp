#include "stdAfx.h" // STL & Windows
#include "boost.h" // boost libraries

#include "WinsockNetwork.h"
#include "SyncLocker.h"

using namespace std;
using namespace boost;

// These are the constants for shutting down sockets for version 1 of the socket
// Version 2 of the socket has different constans (send and receive inverted)
// keep this in mind if we ever switch over
static const int shutdown_receive = 0;
static const int shutdown_send = 1;
static const int shutdown_receive_and_send = 2;

// various contants for ports and message sizes
const int WinsockNetwork::chat_port = 8869;
const int WinsockNetwork::data_port = 8870;
const int WinsockNetwork::maximum_control_packet_size = 4096;
const int WinsockNetwork::maximum_data_packet_size = 100000;

WinsockNetwork::WinsockNetwork() :
	// the control socket
	m_pControlSocket(NULL),
	m_hControlSocket(0),
	m_bControlConnected(false),
	// the data socket
	m_pDataSocket(NULL),
	m_hDataSocket(0),
	m_bDataConnected(false),
	// control members
	m_bIsServer(false),
	m_connectionState(ConnectionState::DISCONNECTED),
	m_bLocalDisconnect(false),	
	m_sEstablished(new CSemaphore(0,1))
{
	InitializeCriticalSection(&m_csControlSocketSend);
	InitializeCriticalSection(&m_csDataSocketSend);
	InitializeCriticalSection(&m_csConnectionState);
	InitializeCriticalSection(&m_csLocalDisconnect);
}

WinsockNetwork::~WinsockNetwork()
{
	DeleteCriticalSection(&m_csControlSocketSend);
	DeleteCriticalSection(&m_csDataSocketSend);
	DeleteCriticalSection(&m_csConnectionState);
	DeleteCriticalSection(&m_csLocalDisconnect);
	delete m_pControlSocket;
	delete m_pDataSocket;
	delete m_sEstablished;

	// kill threads
	if (controlReceiveThread.get() != NULL)
	{
		controlReceiveThread->interrupt();
		controlReceiveThread->join();
		controlReceiveThread.reset();
	}
	
	if (dataReceiveThread.get() != NULL)
	{
		dataReceiveThread->interrupt();
		dataReceiveThread->join();	
		dataReceiveThread.reset();
	}
}

// start listening to for connections
rc_network WinsockNetwork::listen(const std::string& userName)
{
	// set our user name
	this->userName = userName;

	// start listening is an operation that modifies the connection status
	// we must therefore request write access to the connection resource before proceeding
	SyncWriterLock statusLock = SyncWriterLock(m_rwsync_ConnectionStatus);

	// don't try to connect if we are already connected
	if (isConnected())
	{
		return ERROR_ALREADY_CONNECTED;
	}

	// don't try listening if we are already listening
	if (isListening())
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

	// we are now listening, change the state
	setConnectionState(ConnectionState::LISTENING);

	return SUCCESS;
}

rc_network WinsockNetwork::connect(const std::string& ipAddress, const std::string& userName)
{
	// set our user name
	this->userName = userName;

	{
		// connect is an operation that modifies the connection status
		// we must therefore request write access to the connection resource before proceeding
		SyncWriterLock statusLock = SyncWriterLock(m_rwsync_ConnectionStatus);

		// don't try connecting if we are already connected
		if (isConnected())
		{
			return ERROR_ALREADY_CONNECTED;
		}

		// don't try connecting if we are already listening
		if (isListening())
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

		// establish the connection
		setConnectionState(ConnectionState::ESTABLISHING);
		initializeConnection();
	}

	// establish the connection (send user name)
	return establishConnection();
}

rc_network WinsockNetwork::disconnect()
{
	{
		SyncLocker lock(m_csLocalDisconnect);
		// we set this variable so that the peerDisconnect call doesn't notify the UI that the peer has disconnected
		m_bLocalDisconnect = true;
	}

	// disconnect is an operation that modifies the connection status
	// we must therefore request write access to the connection resource before proceeding
	SyncWriterLock statusLock = SyncWriterLock(m_rwsync_ConnectionStatus);

	if (isConnected())
	{
		// it is important to only shutdown the sockets when we are already connected
		// if we close the sockets the other side will get a network error
		// it is only safe to close the sockets once both sides have shutdown the send operations
		// once the other side shuts down its sockets the receive threads will call the peer disconnect function
		// this will give us a clean disconnect
		shutdownSockets();		
	}
	else
	{
		// if we are not connected to anything then it is safe to close our sockets abruptly (no shutdown first)
		terminateConnection();
	}

	return SUCCESS;
}

void WinsockNetwork::peerDisconnect()
{
	// a peer disconnect is an operation that modifies the connection status
	// we must therefore request write access to the connection resource before proceeding
	SyncWriterLock statusLock = SyncWriterLock(m_rwsync_ConnectionStatus);

	if (isConnected())
	{
		SyncLocker lock (m_csLocalDisconnect);
		if (!m_bLocalDisconnect)
		{
			// notify the observers that the peer has disconnected
			// only notify the observers if it was the peer that initiated the disconnect
			notify(PEER_DISCONNECTED);
		}
	}

	// close the sockets and reset the connection
	// we know that the peer has already shutdown it's send operations so it is safe to close the sockets
	terminateConnection();

	return;
}

void WinsockNetwork::networkError(rc_network error)
{
	// a network error is an operation that modifies the connection status
	// we must therefore request write access to the connection resource before proceeding
	SyncWriterLock statusLock = SyncWriterLock(m_rwsync_ConnectionStatus);

	if (isConnected())
	{
		// notify the observers that the network has been disconnected due to an error		
		notify(NETWORK_ERROR, &error);
	}

	// close the sockets and reset the connection
	terminateConnection();

	return;
}

void WinsockNetwork::notifyAccept(NetworkSocket* socket)
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

			// establish the connection
			setConnectionState(ConnectionState::ESTABLISHING);
			initializeConnection();
			establish = true;
		}
	}

	if (establish)
	{
		rc_network error = establishConnection();
		if (error != SUCCESS)
		{
			networkError(error);
		}
		else
		{
			notify(PEER_CONNECTED);
		}
	}

	return;
}

void WinsockNetwork::resetConnectionStatus()
{
	// reset the connection status to not connected
	m_bControlConnected = false;
	m_bDataConnected = false;
	m_bIsServer = false;
	{
		SyncLocker lock (m_csLocalDisconnect);
		m_bLocalDisconnect = false;
	}
	setConnectionState(ConnectionState::DISCONNECTED);
	return;
}

void WinsockNetwork::resetSockets()
{
	shutdownSockets();
	closeSockets();
}

void WinsockNetwork::shutdownSockets()
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

void WinsockNetwork::closeSockets()
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
	delete m_pControlSocket;
	m_pControlSocket = NULL;

	delete m_pDataSocket;
	m_pDataSocket = NULL;

	// kill the threads
	// we don't need to want to wait for the threads to finish by calling join()
	// because the threads can execute this function and that would lead to deadlock
	if (controlReceiveThread.get() != NULL)
	{
		controlReceiveThread->interrupt();
		controlReceiveThread.reset();
	}
	if (dataReceiveThread.get() != NULL)
	{
		dataReceiveThread->interrupt();
		dataReceiveThread.reset();
	}

	return;
}

void WinsockNetwork::controlReceive()
{
	char receivedBuffer[maximum_control_packet_size];
	std::vector<char> queue;
	ControlPacket packet;

	// we need to use flags for operations that require write access to the connection status
	// this thread will use read access to receive data so when a disconnect or error occurs
	// we will release the read access and obtain write access to react to the disconnect/error
	// we do this because a deadlock occurs if a thread tries to obtain both read an write access
	bool peerDisconnected = false;
	bool socketError = false;

	// keep going until the network connection has been terminated,
	// a peer disconnect occurs or a network error occurs
	while (true)
	{
		// look for operations that require write access
		if (peerDisconnected)
		{
			peerDisconnect();
			return;
		}
		else if (socketError)
		{
			networkError(ERROR_SOCKET_ERROR);
			return;
		}

		// receiving data through the control socket is an operation that reads the connection status
		// we must therefore request read access to the connection resource before proceeding
		SyncReaderLock statusLock = SyncReaderLock(m_rwsync_ConnectionStatus);

		ConnectionState_t connectionState = getConnectionState();
		if (connectionState != ConnectionState::CONNECTED &&
			connectionState != ConnectionState::ESTABLISHING)
		{
			// we are no longer connected, we can stop receiving data
			return;
		}

		// sleep to prevent this thread from hogging the CPU (interruption point)
		boost::this_thread::sleep(boost::posix_time::milliseconds(100));

		// receive data through the network
		int rc = recv(m_hControlSocket, (char*) receivedBuffer, maximum_control_packet_size, 0);

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
			peerDisconnected = true;
			continue;
		}

		// add the received bytes to the end of the queue
		queue.insert(queue.end(), &receivedBuffer[0], &receivedBuffer[rc]);

		// the while loop here may seem a little strange but it is needed
		// if the other sends 2 or more messages in quick succession then might read them both
		// in the same recv(...) call, we need to read as many messages as possible from the stream before continuing
		while (packet.readPacket(queue))
		{
			handleControlPacket(packet);

			// start a new packet
			packet = ControlPacket();
		}
	}
	return;
}

void WinsockNetwork::dataReceive()
{
	char receivedBuffer[maximum_data_packet_size];
	std::vector<char> queue;
	DataPacket packet = DataPacket();

	// we need to use flags for operations that require write access to the connection status
	// this thread will use read access to receive data so when a disconnect or error occurs
	// we will release the read access and obtain write access to react to the disconnect/error
	// we do this because a deadlock occurs if a thread tries to obtain both read an write access
	bool peerDisconnected = false;
	bool socketError = false;

	// keep going until the network connection has been terminated,
	// a peer disconnect occurs or a network error occurs
	while (true)
	{
		// look for operations that require write access
		if (peerDisconnected)
		{
			peerDisconnect();
			return;
		}
		else if (socketError)
		{
			networkError(ERROR_SOCKET_ERROR);
			return;
		}		

		// receiving data through the data socket is an operation that reads the connection status
		// we must therefore request read access to the connection resource before proceeding
		SyncReaderLock statusLock = SyncReaderLock(m_rwsync_ConnectionStatus);

		ConnectionState_t connectionState = getConnectionState();
		if (connectionState != ConnectionState::CONNECTED &&
			connectionState != ConnectionState::ESTABLISHING)
		{
			// we are no longer connected, we can stop receiving data
			return;
		}

		// sleep to prevent this thread from hogging the CPU (interruption point)
		boost::this_thread::sleep(boost::posix_time::milliseconds(100));

		// receive data through the network
		int rc = recv(m_hDataSocket, (char*) receivedBuffer, maximum_control_packet_size, 0);

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
			peerDisconnected = true;
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
			handleDataMessage(packet);
		}
	}
	return;
}

rc_network WinsockNetwork::initializeConnection()
{
	// detach the sockets from their threads
	m_hControlSocket = m_pControlSocket->Detach();
	m_hDataSocket = m_pDataSocket->Detach();

	// initialize the socket threads
	controlReceiveThread = auto_ptr<thread>(new thread(boost::bind(&WinsockNetwork::controlReceive, this)));
	dataReceiveThread = auto_ptr<thread>(new thread(boost::bind(&WinsockNetwork::dataReceive, this)));

	return SUCCESS;
}

rc_network WinsockNetwork::establishConnection()
{
	// we need to send our username to establish the connection
	rc_network error = sendUserName(userName);

	if (error != SUCCESS)
	{
		return error;
	}

	// wait 5 seconds for the connection to be established
	if (m_sEstablished->Lock(5000))
	{
		setConnectionState(ConnectionState::CONNECTED);
		return SUCCESS;
	}
	else
	{
		return ERROR_USER_NAME_NOT_RECEIVED;
	}
}

void WinsockNetwork::terminateConnection()
{
	// close the sockets and reset the connection
	resetConnectionStatus();
	resetSockets();
	return;
}

rc_network WinsockNetwork::sendUserName(const std::string& userName)
{
	ControlPacket packet;
	packet.write(CONTROL_PACKET_NAME, userName);
	return syncSendControlMessage(packet);
}

rc_network WinsockNetwork::sendChatMessage(const std::string& message)
{
	ControlPacket packet;
	packet.write(CONTROL_PACKET_CHAT, message);
	return syncSendControlMessage(packet);
}

rc_network WinsockNetwork::sendStartGame()
{
	ControlPacket packet;
	packet.write(CONTROL_PACKET_START_GAME);
	return syncSendControlMessage(packet);
}

rc_network WinsockNetwork::sendPauseGame()
{
	ControlPacket packet;
	packet.write(CONTROL_PACKET_PAUSE_GAME);
	return syncSendControlMessage(packet);
}

rc_network WinsockNetwork::sendEndGame()
{
	ControlPacket packet;
	packet.write(CONTROL_PACKET_END_GAME);
	return syncSendControlMessage(packet);
}

rc_network WinsockNetwork::sendVideoData(const VideoData& video)
{
	DataPacket packet;
	packet.write(DATA_PACKET_VIDEO, video);
	return syncSendDataMessage(packet);
}

rc_network WinsockNetwork::sendPlayerPosition(const cVector3d& position)
{
	DataPacket packet;
	packet.write(DATA_PACKET_PLAYER_POSITION, position);
	return syncSendDataMessage(packet);
}

rc_network WinsockNetwork::sendSlingshotPosition(const cVector3d& position)
{
	DataPacket packet;
	packet.write(DATA_PACKET_SLINGSHOT_POSITION, position);
	return syncSendDataMessage(packet);
}

rc_network WinsockNetwork::sendProjectile(const Projectile& projectile)
{
	DataPacket packet;
	packet.write(DATA_PACKET_PROJECTILE, projectile);
	return syncSendDataMessage(packet);
}

rc_network WinsockNetwork::sendSlingshotPullback()
{
	DataPacket packet;
	packet.write(DATA_PACKET_SLINGSHOT_PULLBACK);
	return syncSendDataMessage(packet);
}

bool WinsockNetwork::isConnected() const
{
	return getConnectionState() == ConnectionState::CONNECTED;
}

bool WinsockNetwork::isListening() const
{
	return getConnectionState() == ConnectionState::LISTENING;
}

rc_network WinsockNetwork::syncSendDataMessage(const DataPacket& packet)
{
	// sending data through the control socket is an operation that reads the connection status
	// we must therefore request read access to the connection resource before proceeding
	SyncReaderLock statusLock = SyncReaderLock(m_rwsync_ConnectionStatus);

	ConnectionState_t connectionState = getConnectionState();
	if (connectionState != ConnectionState::CONNECTED &&
		connectionState != ConnectionState::ESTABLISHING)
	{
		return ERROR_NO_CONNECTION;
	}

	// a message has 2 chunks, header and data
	// the order of the chunks is <header><data>
	vector<shared_ptr<const vector<char> > > message;
	message.push_back(packet.getHeader());
	message.push_back(packet.getData());

	// we need to synchronize here so that only one thread is sending data on the socket at a time
	SyncLocker sendLock = SyncLocker(m_csDataSocketSend);

	foreach (shared_ptr<const vector<char> > chunk, message)
	{
		// skip empty chunks
		if (chunk->size() == 0)
		{
			continue;
		}

		// send the chunk
		// When sending the message we might need to call the send function multiple times.
		// This is because it is not guaranteed to send all the bytes in one packet.
		// We therefore have to keep calling the send function until all the bytes are sent.
		const char* i = (const char*)&chunk->front();
		unsigned int bytesSent = 0;

		while (bytesSent < chunk->size())
		{
			{
				SyncLocker lock (m_csLocalDisconnect);
				if (m_bLocalDisconnect)
				{
					// if we are in the process of disconnecting then we don't want to send anything over this socket
					return ERROR_NO_CONNECTION;
				}
			}

			int rc = send(m_hDataSocket, i, chunk->size() - bytesSent, 0);

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
	}
	return SUCCESS;
}

void WinsockNetwork::handleDataMessage(const DataPacket& message)
{
	switch (message.getType())
	{
		case DATA_PACKET_VIDEO:
		{
			VideoData videoData;
			message.read(videoData);
			notify(RECEIVED_VIDEO, &videoData);			
			break;
		}
		case DATA_PACKET_PLAYER_POSITION:
		{
			cVector3d vec;
			message.read(vec);
			notify(RECEIVED_PLAYER_POSITION, &vec);
			break;
		}
		case DATA_PACKET_SLINGSHOT_POSITION:
		{
			cVector3d vec;
			message.read(vec);
			notify(RECEIVED_SLINGSHOT_POSITION, &vec);			
			break;
		}
		case DATA_PACKET_PROJECTILE:
		{
			Projectile projectile;
			message.read(projectile);
			notify(RECEIVED_PROJECTILE, &projectile);
			break;
		}
		case DATA_PACKET_SLINGSHOT_PULLBACK:
		{
			notify(RECEIVED_PULLBACK);
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

rc_network WinsockNetwork::syncSendControlMessage(const ControlPacket& packet)
{
	// sending data through the control socket is an operation that reads the connection status
	// we must therefore request read access to the connection resource before proceeding
	SyncReaderLock statusLock = SyncReaderLock(m_rwsync_ConnectionStatus);

	ConnectionState_t connectionState = getConnectionState();
	if (connectionState != ConnectionState::CONNECTED &&
		connectionState != ConnectionState::ESTABLISHING)
	{
		return ERROR_NO_CONNECTION;
	}

	// a message has 2 chunks, header and data
	// the order of the chunks is <header><data>
	vector<shared_ptr<const vector<char> > > message;
	message.push_back(packet.getHeader());
	message.push_back(packet.getData());

	// we need to synchronize here so that only one thread is sending data on the socket at a time
	SyncLocker sendLock = SyncLocker(m_csControlSocketSend);

	foreach (shared_ptr<const vector<char> > chunk, message)
	{
		// skip empty chunks
		if (chunk->size() == 0)
		{
			continue;
		}

		// send the chunk
		// When sending the message we might need to call the send function multiple times.
		// This is because it is not guaranteed to send all the bytes in one packet.
		// We therefore have to keep calling the send function until all the bytes are sent.
		const char* i = (const char*)&chunk->front();
		unsigned int bytesSent = 0;

		while (bytesSent < chunk->size())
		{
			{
				SyncLocker lock (m_csLocalDisconnect);
				if (m_bLocalDisconnect)
				{
					// if we are in the process of disconnecting then we don't want to send anything over this socket
					return ERROR_NO_CONNECTION;
				}
			}

			int rc = send(m_hControlSocket, i, chunk->size() - bytesSent, 0);

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
	}
	return SUCCESS;
}

void WinsockNetwork::handleControlPacket(const ControlPacket& packet)
{
	switch(packet.getType())
	{
		case CONTROL_PACKET_NAME:
		{
			// update the remote user name
			string name;
			packet.read(name);
			notify(RECEIVED_USER_NAME, &name);
			{
				if (getConnectionState() == ConnectionState::ESTABLISHING)
				{
					m_sEstablished->Unlock();
				}
			}			
			break;
		}
		case CONTROL_PACKET_CHAT:
		{
			// notify the observers that a new message has arrived
			string message;
			packet.read(message);
			notify(RECEIVED_CHAT_MESSAGE, &message);
			break;
		}
		case CONTROL_PACKET_START_GAME:
		{
			// notify the observers that the peer has started the game
			notify(PEER_START_GAME);
			break;
		}
		case CONTROL_PACKET_PAUSE_GAME:
		{
			// notify the observers that the peer has started the game
			notify(PEER_PAUSE_GAME);
			break;
		}
		case CONTROL_PACKET_END_GAME:
		{
			// notify the observers that the peer has exited the game
			notify(PEER_EXIT_GAME);
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

ConnectionState_t WinsockNetwork::getConnectionState() const
{
	SyncLocker lock(m_csConnectionState);
	return m_connectionState;
}

void WinsockNetwork::setConnectionState(ConnectionState_t state)
{
	SyncLocker lock(m_csConnectionState);
	m_connectionState = state;
	return;
}

