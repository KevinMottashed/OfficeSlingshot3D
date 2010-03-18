// SmartClothingManager.h: interface for the SmartClothingManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SMARTCLOTHINGMANAGER_H__D57F7F21_D039_4BF1_9255_065D47A8F264__INCLUDED_)
#define AFX_SMARTCLOTHINGMANAGER_H__D57F7F21_D039_4BF1_9255_065D47A8F264__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Controller.h"

// forward declarations (files include each other)
class Controller;

class SmartClothingManager  
{
public:
	SmartClothingManager(Controller* pController);
	virtual ~SmartClothingManager();

	// implement this with a proper type for position, probably Vector3D from chai3d
	// implement this with a thread/event/timer
	// the controller shouldn't be blocked while the jacket vibrates (don't put a sleep)
	// if possible, make the time in ms
	void vibrate(int position, int time) {}

private:
	SmartClothingManager(const SmartClothingManager& smartClothingManager); // intentionally not implemented
	SmartClothingManager& operator=(const SmartClothingManager& smartClothingManager); // intentionally not implemented

	Controller* m_pController;
};

#endif // !defined(AFX_SMARTCLOTHINGMANAGER_H__D57F7F21_D039_4BF1_9255_065D47A8F264__INCLUDED_)
