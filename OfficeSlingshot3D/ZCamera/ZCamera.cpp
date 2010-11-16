#include "stdafx.h"
#include "ZCamera.h"

using namespace std;
using namespace boost;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ZCamera::ZCamera()
{
	//Create the depth camera object
	m_depthCamera = new CDepthCamera();

	// Initialize the camera
	zcamPresent = m_depthCamera->Initialize(5000);
}

ZCamera::~ZCamera()
{
	// stop pulling from the camera
	stopCapture();

	//delete resources
	delete m_depthCamera;
}

//////////////////////////////////////////////////////////////////////
// Functions
//////////////////////////////////////////////////////////////////////

//Camera thread to poll for new pictures
void ZCamera::getFrameFromCamera()
{	
	//While the thread is active
	while(true)
	{
		//Get frame from camera, updates the pointers that point to the video data
		m_depthCamera->GetNextFrame(DEPTH, RGBFrame, RGBFull, PRIM, SEC, 1000);		
		
		cVector3d pos = getPlayerPosition();

		notify(AVATAR_POSITION, &pos);

		// sleep so we don't hog CPU (interruption point)
		boost::this_thread::sleep(boost::posix_time::milliseconds(31)); // 32 fps
	}

	return;
}

//Starts the thread that captures frames from the camera
void ZCamera::startCapture() 
{	
	if (!zcamPresent)
	{
		// no camera connected
		return;
	}

	// don't try starting 2 threads
	if (zcameraThread.get() == NULL)
	{
		zcameraThread = auto_ptr<thread>(new thread(boost::bind(&ZCamera::getFrameFromCamera, this)));
	}
}

//stop the thread that captures frames from the camera
void ZCamera::stopCapture() 
{
	if (zcameraThread.get() != NULL)
	{
		zcameraThread->interrupt();
		zcameraThread->join();
		zcameraThread.reset();
	}
}

//Finds the head of the player in the depth image and returns a 3d vector.
cVector3d ZCamera::getPlayerPosition(){

	cVector3d pos;

	//Threshold for how many lines to ignore (to avoid detecting specs and loose hair)
	int rowCount=10;

	//Counters
	int minCol=321;
	int maxCol=-1;
	int finalRow=-1;

	//Go through from top-left to bottom-right. Stop when a finalRow is found
	for (int i=0;i<240 && finalRow<0;i++){
		for (int j=0;j<320;j++){

			//If the pixel is white enough(value>64)
			if ((int)DEPTH[(i*320)+j] > 64){

				//If this is the 10th line with a white pixel, we assume this is the head
				if (rowCount==0){
					finalRow=i;

					//Update counters to find the max and min of white pixels on this line
					if (j<minCol){minCol=j;}
					if (j>maxCol){maxCol=j;}
				}
				
				//If it hasn't been 10 lines with a white pixel, reduce the counter and skip to next line
				else{
					rowCount--;
					break;
				}
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

	//Find the middle white pixel of the horizontal line
	pos.x = -(((maxCol-minCol)/2 + minCol) - 160);
	pos.y = 240 - finalRow;
	pos.z = 0;

	return pos;

}
