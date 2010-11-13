#include "stdafx.h"
#include "SmartClothingManager.h"
#include "DisplayDeviceJacket.h"
#include "DisplayDeviceArmband.h"

using namespace std;
using namespace boost;

bool IS_TACTILE_JACKET_CONNECTED;
bool IS_TACTILE_ARMBAND_CONNECTED;
DisplayDeviceJacket* m_pDisplayDeviceJacket;
DisplayDeviceArmband* m_pDisplayDeviceArmband;
std::auto_ptr<boost::thread> smartClothingThread;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
SmartClothingManager::SmartClothingManager(boost::shared_ptr<Configuration> configuration)
{	
	UserPreferences prefs = configuration->getUserPreferences();

	m_pDisplayDeviceJacket = new DisplayDeviceJacket(prefs.jacketPort);
	IS_TACTILE_JACKET_CONNECTED = m_pDisplayDeviceJacket->initialize();
	
	m_pDisplayDeviceArmband = new DisplayDeviceArmband(prefs.armBandPort);
	IS_TACTILE_ARMBAND_CONNECTED = m_pDisplayDeviceArmband->initialize();
}

SmartClothingManager::~SmartClothingManager()
{
}

void SmartClothingManager::setPorts(int armBandPort, int jacketPort)
{
	IS_TACTILE_JACKET_CONNECTED = m_pDisplayDeviceJacket->setPort(jacketPort);
	IS_TACTILE_ARMBAND_CONNECTED = m_pDisplayDeviceArmband->setPort(armBandPort);
}

void SmartClothingManager::TimerProc()
{
	boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
	if(IS_TACTILE_JACKET_CONNECTED) {
		m_pDisplayDeviceJacket->setIntensityAll(0);
		m_pDisplayDeviceJacket->actuate();
	}
	if(IS_TACTILE_ARMBAND_CONNECTED) {
		m_pDisplayDeviceArmband->setIntensityAll(0);
		m_pDisplayDeviceArmband->actuate();
	}
}

void SmartClothingManager::vibrate(BodyPart_t hitPart, cVector3d position, cVector3d minValue, cVector3d maxValue)
{
	// Take the difference from the x position of the collision and the x minimum value
	// Divide by width of the box to get a value from 0-1
	double xPos = 1 - (position.x - minValue.x)/(maxValue.x - minValue.x);

	// Take the difference from the y position of the collision and the y minimum value
	// Divide by height of the box to get a value from 0-1
	double yPos = 1 - (position.y - minValue.y)/(maxValue.y - minValue.y);

	vibrate(hitPart, xPos, yPos);
}

void SmartClothingManager::vibrate(BodyPart_t touchedPart, double x, double y) 
{
	if(IS_TACTILE_JACKET_CONNECTED) {
		switch(touchedPart) {
			case BodyPart::CHEST:
				m_pDisplayDeviceJacket->setActChest(x,y);
				break;
			case BodyPart::UPPER_LEFT_ARM:
				m_pDisplayDeviceJacket->setActUpperArm(x,y);
				break;
			case BodyPart::HEAD:
				m_pDisplayDeviceJacket->setIntensityAll(1);
				break;
		}
		m_pDisplayDeviceJacket->actuate();
	}

	//if armband is connected, always rumble
	if(IS_TACTILE_ARMBAND_CONNECTED) {
		m_pDisplayDeviceArmband->setIntensityAll(1);
		m_pDisplayDeviceArmband->actuate();
	}

	smartClothingThread = auto_ptr<thread>(new thread(boost::bind(&SmartClothingManager::TimerProc, this)));
}

