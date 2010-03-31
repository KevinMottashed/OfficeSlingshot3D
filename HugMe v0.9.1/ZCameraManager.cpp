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
	//Initiate thread flag to false
	zcam_started = false;

	//Create the depth camera object
	m_depthCamera = new CDepthCamera();

	//Allocate memory for the video frame
	RGB = new BYTE[ZCameraManager::IMAGE_WIDTH*ZCameraManager::IMAGE_HEIGHT*4];
}

ZCameraManager::~ZCameraManager()
{
	//delete resources
	delete m_depthCamera;
	delete[] RGB;
}

//////////////////////////////////////////////////////////////////////
// Functions
//////////////////////////////////////////////////////////////////////

//Camera thread to poll for new pictures
DWORD ZCameraManager::getFrameFromCamera(ZCameraManager* p_ZCamera){
	
	//While the thread is active
	while(p_ZCamera->zcam_started){
		//DO REAL CAMERA STUFF
		
		//GetNextFrame(p_ZCamera->DEPTH,p_ZCamera->RGB,p_ZCamera->RGBFull,p_ZCamera->PRIM,p_ZCamera->SEC,1000);
		//Controller::instance()->notifyNewLocalVideoData((const char*)p_ZCamera->RGB);
	}

	return 0;
}

//Dummy data generator
DWORD ZCameraManager::getFrameFromDummy(ZCameraManager* p_ZCamera){
	
	//Shortcut to access each pixel of every line
	int step = ZCameraManager::IMAGE_WIDTH*4;

	//While the thread is active
	while(p_ZCamera->zcam_started){

		//Go through eahc row and column and fill all 4 values for each pixel
		for (int i=0;i<ZCameraManager::IMAGE_HEIGHT;i++){
			for(int j=0;j<ZCameraManager::IMAGE_WIDTH;j++){
				p_ZCamera->RGB[i*step+j*4]		=	rand()*255;			//blue
				p_ZCamera->RGB[i*step+j*4+1]	=	rand()*255;			//green	
				p_ZCamera->RGB[i*step+j*4+2]	=	rand()*255;			//red
				p_ZCamera->RGB[i*step+j*4+3]	=	0;			//ignored	
			}
		}

		//Send the array to the controller, as remote data so it gets displayed locally
		Controller::instance()->notifyNewRemoteVideoData((const char*)p_ZCamera->RGB);

/*
		cVector3d vec;
		vec.x = rand() % 10;
		vec.y = rand() % 10;
		vec.z = rand() % 10;
		Controller::instance()->notifyNewLocalPlayerPosition(vec);
*/
		Sleep(31); // 32 fps
	}
	return 0;
}

// TODO implement
// start getting pictures from the z camera
// create a thread to poll it
void ZCameraManager::start() {	
	zcam_started = true;

	DWORD threadId;
	HANDLE hThread;

	TDVCameraInterfaceBase* x = m_depthCamera->GetCameraInterface();
	if (x){	//REAL CAMERA is connected
		m_depthCamera->Initialize(1000);
		hThread = CreateThread( 0, 0, (LPTHREAD_START_ROUTINE) getFrameFromCamera,  (void*) this, 0, &threadId);
	}else{	//No camera is connected
		hThread = CreateThread( 0, 0, (LPTHREAD_START_ROUTINE) getFrameFromDummy,  (void*) this, 0, &threadId);
	}
}

// TODO implement
// stop getting pictures
// stop the thread that's polling it
void ZCameraManager::stop() {
	zcam_started = false;
}
