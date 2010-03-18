// Controller.cpp: implementation of the Controller class.
//
//////////////////////////////////////////////////////////////////////

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

void Controller::startGame()
{
	if (m_bGameIsRunning)
	{
		return; // already started
	}
	m_bGameIsRunning = true;
	m_hGameLoopThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) GameLoopThread, (void*) this, 0, &m_dwIDGameLoop);
}

void Controller::exitGame()
{
	m_bGameIsRunning = false;
	m_hGameLoopThread = 0;
	m_dwIDGameLoop = 0;
}

void Controller::closeApplication()
{
	// the only thing to do is to close the sockets so the other end has a clean disconnect
	m_pNetworkManager->disconnect();
}

Controller::Controller() :
		m_hGameLoopThread(0),
		m_dwIDGameLoop(0),
		m_bGameIsRunning(false)
{
	m_pUserInterfaceManager = new UserInterfaceManager(this);
	m_pNetworkManager = new NetworkManager(this);
	m_pFalconPenManager = new FalconPenManager(this);
	m_pZCameraManager = new ZCameraManager(this);
	m_pSmartClothingManager = new SmartClothingManager(this);
	startGame(); // remove this when the UI can start the game
}

Controller::~Controller()
{
	delete m_pNetworkManager;
	delete m_pUserInterfaceManager;
	delete m_pFalconPenManager;
	delete m_pZCameraManager;
	delete m_pSmartClothingManager;
}

void Controller::updateRemoteUserName(const std::string& name)
{
	remoteUserName = name;
	return;
}

std::string Controller::getRemoteUserName()
{
	return remoteUserName;
}

void Controller::updateLocalUserName(const std::string& name)
{
	localUserName = name;
	return;
}

std::string Controller::getLocalUserName()
{
	return localUserName;
}

CDialog* Controller::getMainWindow()
{
	return m_pUserInterfaceManager->getMainWindow();
}

void Controller::notifyNewVideoData(const std::vector<BYTE>& vRGB, const std::vector<BYTE>& vDepth, const std::vector<BYTE>& vAR)
{
	// TODO implement
	printf("new video data has arrived\n");
}

void Controller::notifyNewTactileData(const std::vector<BYTE>& vTactile)
{
	// TODO implement
	printf("new tactile data has arrived\n");
}

void Controller::notifyNewChatMessage(const std::string& message)
{
	m_pUserInterfaceManager->notifyNewChatMessage(message);
}

DWORD Controller::GameLoopThread(Controller* p_Controller)
{
	p_Controller->m_pFalconPenManager->start();
	p_Controller->m_pZCameraManager->start();
	while(p_Controller->m_bGameIsRunning)
	{
		// falcon pen position
		FalconPosition position = p_Controller->m_pFalconPenManager->getSlingshotPosition();
		std::cout << "The slingshot is at " << position.getPosition()
				<< " with an orientation of " << position.getOrientation() << std::endl;

		// falcon pen projectile
		if (p_Controller->m_pFalconPenManager->hasNewProjectile())
		{
			Projectile projectile = p_Controller->m_pFalconPenManager->getNewProjectile();
			std::cout << "A new projectile was created at " << projectile.getPosition()
					<< " with a velocity of " << projectile.getVelocity() << std::endl;
		}

		// rgb data
		std::vector<BYTE> rgb = p_Controller->m_pZCameraManager->getFrame();
		std::cout << "We has received " << rgb.size() << " bytes of video data" << std::endl;

		// the player position
		std::cout << "The player is now located at "
				<< p_Controller->m_pZCameraManager->getPlayerPosition() << std::endl;

		// vibrate the jacket
		p_Controller->m_pSmartClothingManager->vibrate(1, 10);

		// sleep for a short while
		Sleep(1000); // 1 sec
	}
	p_Controller->m_pFalconPenManager->stop();
	p_Controller->m_pZCameraManager->stop();
	return 0;
}

