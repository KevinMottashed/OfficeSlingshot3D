// NetworkManager.cpp: implementation of the NetworkManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NetworkManager.h"

const int NetworkManager::chat_port = 8869;

NetworkManager::NetworkManager(Controller* controller) :
	m_pChatSocket(NULL), m_pController(controller)
{
}

NetworkManager::~NetworkManager()
{
	delete m_pChatSocket;
}

// start listening to for connections
rc_network NetworkManager::startListening()
{
	if (m_pChatSocket != NULL)
	{
		return ERROR_ALREADY_LISTENING;
	}

	m_pChatSocket = new CChatSocket(this);
	if (!m_pChatSocket->Create(chat_port))
	{
		return ERROR_CREATE_SOCKET;
	}

	if (!m_pChatSocket->Listen())
	{
		return ERROR_LISTEN_SOCKET;
	}
	return SUCCESS;
}

void NetworkManager::notifyAccept()
{
	m_pController->notifyNetworkConnectionAccepted();
	return;
}

