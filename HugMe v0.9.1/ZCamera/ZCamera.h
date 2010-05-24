#ifndef ZCAMERA_H
#define ZCAMERA_H

#include "stdafx.h"
#include "chai3d.h"
#include "DepthCamera.h"
#include "IZCamera.h"
#include "VideoData.h"
#include "boost.h"

// concrete implementation of the z camera manager
class ZCamera : public IZCamera 
{
public:	
	ZCamera();
	virtual ~ZCamera();

	// create a thread to poll the camera
	void start();

	// stop the thread that's polling the camera
	void stop();
	
	// Camera loop that fetches frames and player position.
	// runs at 32 fps and continually sends information to controller
	static DWORD getFrameFromCamera(ZCamera* p_ZCamera);
	static DWORD getFrameFromDummy(ZCamera* p_ZCamera);

	//Reverse the image array, so it displays correctly on the UI
	static void reverseFrameUpDown(VideoData vd, int channels);	
	static void reverseFrameLeftRight(VideoData vd, int channels);	

private:
	ZCamera(const ZCamera& zCamera); // intentionally not implemented
	ZCamera& operator=(const ZCamera& zCamera); // intentionally not implemented

	bool zcam_started;

	CDepthCamera * m_depthCamera;
	boost::shared_ptr<std::vector<BYTE> > RGB;
	unsigned char* DEPTH;
	unsigned char* RGBFull;
	unsigned char* PRIM;
	unsigned char* SEC;

};

#endif
 
