// ZCameraManager.h: interface for the ZCameraManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ZCAMERAMANAGER_H__2520671A_E135_424D_8BBB_12E57375D836__INCLUDED_)
#define AFX_ZCAMERAMANAGER_H__2520671A_E135_424D_8BBB_12E57375D836__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "StdAfx.h"
#include "Controller.h"
#include "CVector3d.h"

// forward declarations (files include each other)
class Controller;

class ZCameraManager  
{
public:
	ZCameraManager(Controller* pController);
	virtual ~ZCameraManager();

	// TODO implement
	// start getting pictures from the z camera
	// create a thread to poll it
	void start();

	// TODO implement
	// stop getting pictures
	// stop the thread that's polling it
	void stop();
	
	// Camera loop that fetches frames and player position.
	// runs at 32 fps and continually sends information to controller
	static DWORD getFrameFromCamera(ZCameraManager* p_ZCamera);

private:
	ZCameraManager(const ZCameraManager& zCameraManager); // intentionally not implemented
	ZCameraManager& operator=(const ZCameraManager& zCameraManager); // intentionally not implemented

	Controller* m_pController;
	bool zcam_started;
	std::vector<BYTE> currentFrame;

};

#endif // !defined(AFX_ZCAMERAMANAGER_H__2520671A_E135_424D_8BBB_12E57375D836__INCLUDED_)
