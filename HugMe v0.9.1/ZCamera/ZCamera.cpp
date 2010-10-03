#include "stdafx.h"
#include "ZCamera.h"

using namespace std;
using namespace boost;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ZCamera::ZCamera()
{
	//Initiate thread flag to false
	zcam_started = false;

	//Create the depth camera object
	m_depthCamera = new CDepthCamera();

	//Allocate memory for the video frame
	RGB = shared_ptr<vector<BYTE> >(new vector<BYTE>(IMAGE_ARRAY_SIZE));
}

ZCamera::~ZCamera()
{
	//delete resources
	delete m_depthCamera;
}

//////////////////////////////////////////////////////////////////////
// Functions
//////////////////////////////////////////////////////////////////////

//Camera thread to poll for new pictures
DWORD ZCamera::getFrameFromCamera(ZCamera* p_ZCamera){
	
	//While the thread is active
	while(p_ZCamera->zcam_started)
	{
		VideoData video;

		// get a pointer to the video array
		BYTE* rgb = &video.rgb.front();
		
		//Get frame from camera, updates the values of the char arrays
		p_ZCamera->m_depthCamera->GetNextFrame(p_ZCamera->DEPTH,rgb,p_ZCamera->RGBFull,p_ZCamera->PRIM,p_ZCamera->SEC,1000);		

		//Flip the image up-down
	//	reverseFrameUpDown(video,4);

		//Send to the observers
	//	p_ZCamera->notify(VIDEO, &video);

		
		cVector3d pos = getPlayerPosition(p_ZCamera, 0);

		p_ZCamera->notify(AVATAR_POSITION, &pos);

		Sleep(31); // 32 fps
	}

	return 0;
}

//Dummy data generator
DWORD ZCamera::getFrameFromDummy(ZCamera* p_ZCamera){
	
	p_ZCamera->dummyCounter=0;

	//While the thread is active
	while(p_ZCamera->zcam_started){

		p_ZCamera->dummyCounter++;

		cVector3d pos = getPlayerPosition(p_ZCamera, 0);

		p_ZCamera->notify(AVATAR_POSITION, &pos);

		Sleep(31); // 32 fps
	}
	return 0;
}

//Starts the thread that captures frames from the camera
void ZCamera::startCapture() {	
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

//stop the thread that captures frames from the camera
void ZCamera::stopCapture() {
	zcam_started = false;
}

//Reverses the image up-down
void ZCamera::reverseFrameUpDown(VideoData& vd,int channels){

	BYTE* RGB = &vd.rgb.front();

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
void ZCamera::reverseFrameLeftRight(VideoData& vd,int channels){

	BYTE* RGB = &vd.rgb.front();

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


//Finds the head of the player in the depth image and returns a 3d vector.
cVector3d ZCamera::getPlayerPosition(ZCamera* p_ZCamera, bool isCameraConnected){

	cVector3d pos;

	if (isCameraConnected){

		int rowCount=10;

		int minCol=321;
		int maxCol=-1;
		int finalRow=-1;

		for (int i=0;i<240 && finalRow<0;i++){
			for (int j=0;j<320;j++){
				if ((int)p_ZCamera->DEPTH[(i*320)+j] > 64){

					if (rowCount==0){
						finalRow=i;
						if (j<minCol){minCol=j;}
						if (j>maxCol){maxCol=j;}
					}else{
						rowCount--;
						break;
					}
				}
				else{
				}
			}
		}
		
		/* Testing X Y values - Could be removed, but I'm keeping to test camera
		ofstream myfile2;
		myfile2.open ("c://DAN_HEAD.txtwordpad");
		myfile2<< "x: " << ((maxCol-minCol)/2 + minCol) - 160;
		myfile2<< "\n";
		myfile2<< "y: " << 240 - finalRow;
		myfile2.close();
		*/

		pos.x = ((maxCol-minCol)/2 + minCol) - 160;
		pos.y = 240 - finalRow;
		pos.z = 0;
	
	}
	else
	{
		//Number of "camera pixels" we want the body to move left to right
		int rangeOfMovement=260;

		pos.x = (p_ZCamera->dummyCounter % rangeOfMovement) - rangeOfMovement/2;
		pos.y = 120;
		pos.z = 0;
	}

	return pos;

}
