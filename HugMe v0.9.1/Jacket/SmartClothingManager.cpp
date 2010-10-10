#include "stdafx.h"
#include "SmartClothingManager.h"
#include "DisplayDeviceJacket.h"
#include "DisplayDeviceArmband.h"

bool IS_TACTILE_JACKET_CONNECTED;
bool IS_TACTILE_ARMBAND_CONNECTED;
DisplayDeviceJacket* m_pDisplayDeviceJacket;
DisplayDeviceArmband* m_pDisplayDeviceArmband;

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


VOID CALLBACK MySetTimerProc(
  HWND hwnd,     // handle of window for timer messages
  UINT uMsg,     // WM_TIMER message
  UINT idEvent,  // timer identifier
  DWORD dwTime   // current system time
)
{
	if(IS_TACTILE_JACKET_CONNECTED) {
		m_pDisplayDeviceJacket->setIntensityAll(0);
		m_pDisplayDeviceJacket->actuate();
	}
	if(IS_TACTILE_ARMBAND_CONNECTED) {
		m_pDisplayDeviceArmband->setIntensityAll(0);
		m_pDisplayDeviceArmband->actuate();
	}

	KillTimer(hwnd,idEvent);
}

void SmartClothingManager::setPorts(int armBandPort, int jacketPort)
{
	m_pDisplayDeviceJacket->setPort(jacketPort);
	m_pDisplayDeviceArmband->setPort(armBandPort);
}

void SmartClothingManager::vibrate(HumanPart touchedPart, int x, int y, int time) 
{
	//if(m_pHugMe->m_localHumanModel.m_contactInfo.isContacted) {
		switch(touchedPart) {
		case CHEST:
			if(IS_TACTILE_JACKET_CONNECTED) {
				m_pDisplayDeviceJacket->setActChest(x,y);
				m_pDisplayDeviceJacket->actuate();
			}
		break;
		case RIGHT_UPPER_ARM:
			if(IS_TACTILE_JACKET_CONNECTED) {
				m_pDisplayDeviceJacket->setActUpperArm(x,y);
				m_pDisplayDeviceJacket->actuate();
			}
		break;
		case RIGHT_LOWER_ARM:
			if(IS_TACTILE_ARMBAND_CONNECTED) {
				m_pDisplayDeviceArmband->setActForearm(x,y);
				m_pDisplayDeviceArmband->actuate();
			}
		break;
		}

		SetTimer(NULL,1, 2000, MySetTimerProc);
}

