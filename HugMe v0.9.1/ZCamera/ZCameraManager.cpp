#include "stdafx.h"
#include "ZCameraManager.h"

using namespace std;
using namespace boost;

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
	RGB = shared_ptr<vector<BYTE> >(new vector<BYTE>(IMAGE_ARRAY_SIZE));
}

ZCameraManager::~ZCameraManager()
{
	//delete resources
	delete m_depthCamera;
}

//////////////////////////////////////////////////////////////////////
// Functions
//////////////////////////////////////////////////////////////////////

//Camera thread to poll for new pictures
DWORD ZCameraManager::getFrameFromCamera(ZCameraManager* p_ZCamera){
	
	//While the thread is active
	while(p_ZCamera->zcam_started){

		// get a pointer to the video array
		BYTE* rgb = &p_ZCamera->RGB->front();
		
		//Get frame from camera, updates the values of the char arrays
		p_ZCamera->m_depthCamera->GetNextFrame(p_ZCamera->DEPTH,rgb,p_ZCamera->RGBFull,p_ZCamera->PRIM,p_ZCamera->SEC,1000);
		
		VideoData video(p_ZCamera->RGB);

		//Flip the image up-down
		reverseFrameUpDown(video,4);

		//Send to the observers
		p_ZCamera->notify(VIDEO, &video);

		Sleep(31); // 32 fps
	}

	return 0;
}

//Dummy data generator
DWORD ZCameraManager::getFrameFromDummy(ZCameraManager* p_ZCamera){
	
	//Shortcut to access each pixel of every line
	int step = IMAGE_WIDTH*4;

	int diff_x = 0;
	bool moving_right_x = true;
	int diff_y = 0;
	bool moving_up_y = true;

	//While the thread is active
	while(p_ZCamera->zcam_started){

		// get a pointer to the video array
		BYTE* rgb = &p_ZCamera->RGB->front();

		if (moving_right_x){
			diff_x += 4;
			if (diff_x>30){
				moving_right_x = false;
			}
		}else{
			diff_x -= 4;
			if (diff_x<0){
				moving_right_x = true;
			}
		}

		if (moving_up_y){
			diff_y += 4;
			if (diff_y>20){
				moving_up_y = false;
			}
		}else{
			diff_y -= 4;
			if (diff_y<0){
				moving_up_y = true;
			}
		}

		//Go through each row and column and fill all 4 values for each pixel
		for (int i=0;i<IMAGE_HEIGHT;i++){
			for(int j=0;j<IMAGE_WIDTH;j++){
				if ((i>(IMAGE_HEIGHT*0.66 -15+diff_x) && j>(IMAGE_WIDTH*0.15 -10+diff_y) && j<(IMAGE_WIDTH*0.85 -10+diff_y)	)||(	//BODY
					 i>(IMAGE_HEIGHT*0.33 -15+diff_x) && j>(IMAGE_WIDTH*0.35 -10+diff_y) && j<(IMAGE_WIDTH*0.65 -10+diff_y)			//HEAD
					)){
					rgb[i*step+j*4]		=	0;			//blue
					rgb[i*step+j*4+1]	=	0;			//green	
					rgb[i*step+j*4+2]	=	255;			//red
					rgb[i*step+j*4+3]	=	1;			//will be used for collision detection	
				}else{
					rgb[i*step+j*4]		=	255;			//blue
					rgb[i*step+j*4+1]	=	0;			//green	
					rgb[i*step+j*4+2]	=	0;			//red
					rgb[i*step+j*4+3]	=	0;			//will be used for collision detection		
				}
			
			}
		}

		VideoData video(p_ZCamera->RGB);
		reverseFrameUpDown(video,4);

		// notify the Mediator that new local video data has arrived
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
	//TODO: Change 500 delay time to 5000 when we expect the zcam to be connected. (Reduced wait time for the summer)
	if (m_depthCamera->Initialize(500)){
		hThread = CreateThread( 0, 0, (LPTHREAD_START_ROUTINE) getFrameFromCamera,  (void*) this, 0, &threadId);
	}else{
		hThread = CreateThread( 0, 0, (LPTHREAD_START_ROUTINE) getFrameFromDummy,  (void*) this, 0, &threadId);
	}
}

//Stops the thread from running
void ZCameraManager::stop() {
	zcam_started = false;
}

//Reverses the image up-down
void ZCameraManager::reverseFrameUpDown(VideoData vd,int channels){

	BYTE* RGB = &(vd.rgb)->front();

	int step = IMAGE_WIDTH*channels;
	unsigned char* tmp = new BYTE[step];

	for(int i=0; i<IMAGE_HEIGHT/2; i++){
		memcpy(tmp,RGB + step*(IMAGE_HEIGHT-i-1),step);				//tmp = b
		memcpy(RGB + step*(IMAGE_HEIGHT-i-1),RGB+i*step,step);		//b = a
		memcpy(RGB+i*step,tmp,step);												//a = tmp
	}

	delete []tmp;

}

//Reverses the image left-right
void ZCameraManager::reverseFrameLeftRight(VideoData vd,int channels){

	BYTE* RGB = &(vd.rgb)->front();

	int step = channels;
	int line_step = 0;
	unsigned char* tmp = new BYTE[step];

	for(int i=0; i<IMAGE_HEIGHT; i++){
		line_step = step * i * IMAGE_WIDTH;
		for (int j=0;j<IMAGE_WIDTH/2;j++){
			memcpy(tmp,RGB + line_step + step*(IMAGE_WIDTH-j-1),step);				//tmp = b
			memcpy(RGB + line_step + step*(IMAGE_WIDTH-j-1),RGB + line_step + step*j,step);		//b = a
			memcpy(RGB + line_step + step*j,tmp,step);								//a = tmp
		}
	}

	delete []tmp;

}
