// SmartClothingManager.h: interface for the SmartClothingManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SMARTCLOTHINGMANAGER_H__D57F7F21_D039_4BF1_9255_065D47A8F264__INCLUDED_)
#define AFX_SMARTCLOTHINGMANAGER_H__D57F7F21_D039_4BF1_9255_065D47A8F264__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MediatorProxy.h"

//Should be created in HumanModel... 
typedef enum tagHumanPart {
	CHEST = 0,
	RIGHT_UPPER_ARM,
	RIGHT_LOWER_ARM,
	TOTAL_NUM_PART
} HumanPart;

class SmartClothingManager  
{
public:
	SmartClothingManager();
	virtual ~SmartClothingManager();

	// implement this with a proper type for position, probably Vector3D from chai3d
	// implement this with a thread/event/timer
	// the Mediator shouldn't be blocked while the jacket vibrates (don't put a sleep)
	// if possible, make the time in ms
	void vibrate(HumanPart touchedPart, int x, int y, int time);

private:
	SmartClothingManager(const SmartClothingManager& smartClothingManager); // intentionally not implemented
	SmartClothingManager& operator=(const SmartClothingManager& smartClothingManager); // intentionally not implemented
};

#endif // !defined(AFX_SMARTCLOTHINGMANAGER_H__D57F7F21_D039_4BF1_9255_065D47A8F264__INCLUDED_)
