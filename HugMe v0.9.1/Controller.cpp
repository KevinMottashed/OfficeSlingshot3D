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

void Controller::notifyPeerDisconnected()
{
	// notify the user interface that the network connection as been disconnected
	m_pUserInterfaceManager->notifyPeerDisconnected();
}

void Controller::notifyNetworkError()
{
	// notify the user interface that the network connection as been disconnected
	m_pUserInterfaceManager->notifyNetworkError();
}

rc_network Controller::netConnect(const CString& ipAddress)
{
	return m_pNetworkManager->connect(ipAddress);
}

rc_network Controller::netDisconnect()
{
	return m_pNetworkManager->disconnect();
}

void Controller::startVideo()
{
	//TODO Implement this in the video controller maybe
}

void Controller::exitVideo()
{
	//TODO Implement this in the video controller maybe
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

string Controller::getRemoteUserName()
{
	return remoteUserName;
}

void Controller::updateLocalUserName(const std::string& name)
{
	localUserName = name;
	return;
}

string Controller::getLocalUserName()
{
	return localUserName;
}

CDialog* Controller::getMainWindow()
{
	return m_pUserInterfaceManager->getMainWindow();
}

