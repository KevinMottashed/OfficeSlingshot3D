// ZCameraManager.h: interface for the ZCameraManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ZCAMERAMANAGER_H__2520671A_E135_424D_8BBB_12E57375D836__INCLUDED_)
#define AFX_ZCAMERAMANAGER_H__2520671A_E135_424D_8BBB_12E57375D836__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "StdAfx.h"
#include "chai3d.h"
#include "DepthCamera.h"
#include "ZCameraSubject.h"
#include "VideoData.h"

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
	unsigned char* RGB;
	unsigned char* DEPTH;
	unsigned char* RGBFull;
	unsigned char* PRIM;
	unsigned char* SEC;

};

#endif // !defined(AFX_ZCAMERAMANAGER_H__2520671A_E135_424D_8BBB_12E57375D836__INCLUDED_)
