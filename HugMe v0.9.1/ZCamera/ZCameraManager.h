#ifndef ZCAMERA_MANAGER_H
#define ZCAMERA_MANAGER_H

#include "StdAfx.h"
#include "chai3d.h"
#include "DepthCamera.h"
#include "ZCameraSubject.h"
#include "VideoData.h"
#include "boost.h"

class ZCameraManager : public ZCameraSubject 
{
public:	

	ZCameraManager();
	virtual ~ZCameraManager();

	// create a thread to poll the camera
	void start();

	// stop the thread that's polling the camera
	void stop();
	
	// Camera loop that fetches frames and player position.
	// runs at 32 fps and continually sends information to controller
	static DWORD getFrameFromCamera(ZCameraManager* p_ZCamera);
	static DWORD getFrameFromDummy(ZCameraManager* p_ZCamera);

	//Reverse the image array, so it displays correctly on the UI
	static void reverseFrame(unsigned char* &RGB, int channels);	

private:
	ZCameraManager(const ZCameraManager& zCameraManager); // intentionally not implemented
	ZCameraManager& operator=(const ZCameraManager& zCameraManager); // intentionally not implemented

	bool zcam_started;

	CDepthCamera * m_depthCamera;
	boost::shared_ptr<std::vector<BYTE> > RGB;
	unsigned char* DEPTH;
	unsigned char* RGBFull;
	unsigned char* PRIM;
	unsigned char* SEC;

};

#endif
 
