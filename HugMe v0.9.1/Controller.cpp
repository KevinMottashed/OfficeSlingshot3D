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
	m_pUserInterfaceManager->notifyNetworkConnectionEstablished();
}

rc_network Controller::netConnect(const CString& ipAddress)
{
	return m_pNetworkManager->connect(ipAddress);
}

Controller::Controller()
{
	m_pUserInterfaceManager = new UserInterfaceManager(this);
	m_pNetworkManager = new NetworkManager(this);
}

Controller::~Controller()
{
	delete m_pNetworkManager;
}

void Controller::updateRemoteUserName(const std::string& name)
{
	remoteUserName = name;
	return;
}

CDialog* Controller::getMainWindow()
{
	return m_pUserInterfaceManager->getMainWindow();
}

