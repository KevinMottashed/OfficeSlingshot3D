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

NetworkManager::NetworkManager(Controller* controller) :
	m_pControlSocket(NULL), m_hControlSocket(0), m_bControlConnected(false),
			m_pDataSocket(NULL), m_hDataSocket(0), m_bDataConnected(false),
			m_pController(controller),
			m_sControlSocketSend(0, maximum_messages), m_sControlSocketReceive(
					0, maximum_messages)
{
	InitializeCriticalSection(&m_csControlSocketSend);
	InitializeCriticalSection(&m_csControlSocketReceive);
}

NetworkManager::~NetworkManager()
{
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

rc_network NetworkManager::connect(CString ipAddress)
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

void NetworkManager::closeSockets()
{
	// reset the connection status to not connected
	m_bControlConnected = false;
	m_bDataConnected = false;
	m_bIsConnected = false;
	m_bIsServer = false;

	// Close sockets
	if (m_hControlSocket)
	{
		shutdown(m_hControlSocket, 0);
		closesocket(m_hControlSocket);
		m_hControlSocket = 0;
	}

	if (m_hDataSocket)
	{
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

		// the message to send
		std::vector<BYTE> message;

		// need to synchronize access to the send queue,
		// another thread might try to add a message to the queue while we send
		synchronized (pNetworkManager->m_csControlSocketSend)
		{
			// get the message at the front of the queue
			std::vector<BYTE> messageToSend = pNetworkManager->m_ControlSocketSendQueue.front();

			// copy it into our buffer
			message.insert(message.end(), messageToSend.begin(), messageToSend.end());

			// remove the message from the queue of messages to send
			pNetworkManager->m_ControlSocketSendQueue.pop();
		}

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
					return 0;
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
	//BYTE receivedBuffer[76800]; // not correct, TODO find correct size
	vector<BYTE> queue;
	while (pNetworkManager->m_bIsConnected)
	{
		// sleep to prevent this thread from hogging the CPU
		Sleep(1);
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

	std::vector<BYTE> nameMessage;
	nameMessage.push_back('A');
	nameMessage.push_back('B');
	nameMessage.push_back('C');

	sendControlMessage(nameMessage);

	// notify the controller that the connection has been accepted
	m_pController->notifyNetworkConnectionAccepted();

	return SUCCESS;
}

rc_network NetworkManager::sendControlMessage(const std::vector<BYTE>& message)
{
	// we synchronize here to avoid adding a message to the queue
	// while a message is being sent
	synchronized(m_csControlSocketSend)
	{
		// add the message to the rear of the queue
		m_ControlSocketSendQueue.push(message);
	}

	// notifies the send queue that there is a message to send
	m_sControlSocketSend.Unlock();

	return SUCCESS;
}

