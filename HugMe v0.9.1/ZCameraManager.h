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
	void start() {}

	// TODO implement
	// stop getting pictures
	// stop the thread that's polling it
	void stop() {}

private:
	ZCameraManager(const ZCameraManager& zCameraManager); // intentionally not implemented
	ZCameraManager& operator=(const ZCameraManager& zCameraManager); // intentionally not implemented

	Controller* m_pController;

};

#endif // !defined(AFX_ZCAMERAMANAGER_H__2520671A_E135_424D_8BBB_12E57375D836__INCLUDED_)
