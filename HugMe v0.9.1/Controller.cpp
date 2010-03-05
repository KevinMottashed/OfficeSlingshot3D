// Controller.cpp: implementation of the Controller class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Controller.h"

Controller* Controller::globalInstance = NULL;

Controller* Controller::instance()
{
	if (globalInstance == NULL)
	{
		globalInstance = new Controller();
	}
	return globalInstance;
}

rc_network Controller::netStartListening()
{
	return m_pNetworkManager->startListening();
}

void Controller::notifyNetworkConnectionAccepted()
{
	// send the connection accepted message to the user interface process
	m_pChatWindow->SendMessage(WM_ON_ACCEPT, NULL, NULL);
}

void Controller::setChatWindow(CWnd *pChatWindow)
{
	m_pChatWindow = pChatWindow;
	return;
}

rc_network Controller::netConnect(CString ipAddress)
{
	return m_pNetworkManager->connect(ipAddress);
}

Controller::Controller()
{
	m_pNetworkManager = new NetworkManager(this);
}

Controller::~Controller()
{
	delete m_pNetworkManager;
}
