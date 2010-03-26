// ZCameraManager.cpp: implementation of the ZCameraManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ZCameraManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ZCameraManager::ZCameraManager()
{
		zcam_started = false;
}

ZCameraManager::~ZCameraManager()
{
}

//////////////////////////////////////////////////////////////////////
// Functions
//////////////////////////////////////////////////////////////////////

//Dummy data generator
DWORD ZCameraManager::getFrameFromCamera(ZCameraManager* p_ZCamera){
	
	std::vector<BYTE> nextFrame;

	int frameColor = 0;

	while(p_ZCamera->zcam_started){
		nextFrame.clear();

		//Generate a vector of 320X240 times 3 pixels for each of the RGB
		int IMAGE_WIDTH = 320;
		int IMAGE_HEIGTH = 240;

		for (int i=0;i<IMAGE_WIDTH*IMAGE_HEIGTH;i++){
			if (frameColor % 3 == 0)
			{
				nextFrame.push_back(255);
				nextFrame.push_back(0);
				nextFrame.push_back(0);
			}
			else if (frameColor % 3 == 1)
			{
				nextFrame.push_back(0);
				nextFrame.push_back(255);
				nextFrame.push_back(0);
			}
			else
			{
				nextFrame.push_back(0);
				nextFrame.push_back(0);
				nextFrame.push_back(255);
			}
		}

		++frameColor;

		p_ZCamera->currentFrame = nextFrame;
		Controller::instance()->notifyNewLocalVideoData(p_ZCamera->currentFrame);


		cVector3d vec;
		vec.x = 5;
		vec.y = 5;
		vec.z = 5;
		Controller::instance()->notifyNewLocalPlayerPosition(vec);

		Sleep(31); // 32 fps
	}
	p_ZCamera->currentFrame.clear();
	return 0;
}

// TODO implement
// start getting pictures from the z camera
// create a thread to poll it
void ZCameraManager::start() {	
	zcam_started = true;

	DWORD threadId;
	HANDLE hThread = CreateThread( 0, 0, (LPTHREAD_START_ROUTINE) getFrameFromCamera,  (void*) this, 0, &threadId);
}

// TODO implement
// stop getting pictures
// stop the thread that's polling it
void ZCameraManager::stop() {
	zcam_started = false;
}
