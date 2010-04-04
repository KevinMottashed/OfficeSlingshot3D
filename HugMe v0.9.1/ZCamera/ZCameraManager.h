// ZCameraManager.h: interface for the ZCameraManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ZCAMERAMANAGER_H__2520671A_E135_424D_8BBB_12E57375D836__INCLUDED_)
#define AFX_ZCAMERAMANAGER_H__2520671A_E135_424D_8BBB_12E57375D836__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "StdAfx.h"
#include "ControllerProxy.h"
#include "chai3d.h"
#include "DepthCamera.h"

// forward declarations (files include each other)
class Controller;

class ZCameraManager  
{

public:

	static const unsigned int IMAGE_WIDTH = 320;
	static const unsigned int IMAGE_HEIGHT = 240;
	static const unsigned int IMAGE_ARRAY_SIZE = IMAGE_WIDTH * IMAGE_HEIGHT * 4;

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

	
	unsigned char* RGB;

private:
	ZCameraManager(const ZCameraManager& zCameraManager); // intentionally not implemented
	ZCameraManager& operator=(const ZCameraManager& zCameraManager); // intentionally not implemented

	bool zcam_started;

	CDepthCamera * m_depthCamera;
	unsigned char* DEPTH;
	unsigned char* RGBFull;
	unsigned char* PRIM;
	unsigned char* SEC;

};

#endif // !defined(AFX_ZCAMERAMANAGER_H__2520671A_E135_424D_8BBB_12E57375D836__INCLUDED_)