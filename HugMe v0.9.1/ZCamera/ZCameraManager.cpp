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
	RGB = new BYTE[IMAGE_ARRAY_SIZE];
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
		
		//Get frame from camera, updates the values of the char arrays
		p_ZCamera->m_depthCamera->GetNextFrame(p_ZCamera->DEPTH,p_ZCamera->RGB,p_ZCamera->RGBFull,p_ZCamera->PRIM,p_ZCamera->SEC,1000);
		
		//Flip the image up-down
		reverseFrame(p_ZCamera->RGB,4);

		//Send to the observers
		VideoData video((const char*) p_ZCamera->RGB, IMAGE_WIDTH, IMAGE_HEIGHT);
		p_ZCamera->notify(VIDEO, &video);

		Sleep(31); // 32 fps
	}

	return 0;
}

//Dummy data generator
DWORD ZCameraManager::getFrameFromDummy(ZCameraManager* p_ZCamera){
	
	//Shortcut to access each pixel of every line
	int step = IMAGE_WIDTH*4;

	//While the thread is active
	while(p_ZCamera->zcam_started){

		//Go through each row and column and fill all 4 values for each pixel
		for (int i=0;i<IMAGE_HEIGHT;i++){
			for(int j=0;j<IMAGE_WIDTH;j++){
				p_ZCamera->RGB[i*step+j*4]		=	rand() % 255;			//blue
				p_ZCamera->RGB[i*step+j*4+1]	=	rand() % 255;			//green	
				p_ZCamera->RGB[i*step+j*4+2]	=	rand() % 255;			//red
				p_ZCamera->RGB[i*step+j*4+3]	=	0;			//ignored	
			}
		}

		reverseFrame(p_ZCamera->RGB,4);

		// notify the Mediator that new local video data has arrived
		VideoData video((const char*) p_ZCamera->RGB, IMAGE_WIDTH, IMAGE_HEIGHT);
		p_ZCamera->notify(VIDEO, &video);
/*
		cVector3d vec;
		vec.x = rand() % 10;
		vec.y = rand() % 10;
		vec.z = rand() % 10;
		Mediator::instance()->notifyNewLocalPlayerPosition(vec);
*/
		Sleep(31); // 32 fps
	}
	return 0;
}

//Starts the thread and begins to send data to the Mediator
void ZCameraManager::start() {	
	zcam_started = true;

	DWORD threadId;
	HANDLE hThread;

	//Check if the camera is connection, if not we use dummy data for frames.
	if (m_depthCamera->Initialize(5000)){
		hThread = CreateThread( 0, 0, (LPTHREAD_START_ROUTINE) getFrameFromCamera,  (void*) this, 0, &threadId);
	}else{
		hThread = CreateThread( 0, 0, (LPTHREAD_START_ROUTINE) getFrameFromDummy,  (void*) this, 0, &threadId);
	}
}

//Stops the thread from running
void ZCameraManager::stop() {
	zcam_started = false;
}

//Reverses the image up-down, but not left-right
void ZCameraManager::reverseFrame(unsigned char* &RGB,int channels){

	int step = IMAGE_WIDTH*channels;
	unsigned char* tmp = new BYTE[step];

	for(int i=0; i<IMAGE_HEIGHT/2; i++){
		memcpy(tmp,RGB + step*(IMAGE_HEIGHT-i-1),step);				//tmp = b
		memcpy(RGB + step*(IMAGE_HEIGHT-i-1),RGB+i*step,step);		//b = a
		memcpy(RGB+i*step,tmp,step);												//a = tmp
	}

	delete []tmp;

}
