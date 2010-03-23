// FalconPenManager.h: interface for the FalconPenManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FALCONPENMANAGER_H__AFCF2776_E117_47EC_94C7_97838C3AFB59__INCLUDED_)
#define AFX_FALCONPENMANAGER_H__AFCF2776_E117_47EC_94C7_97838C3AFB59__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Controller.h"

// forward declarations (files include each other)
class Controller;

class FalconPenManager  
{
public:
	FalconPenManager();
	virtual ~FalconPenManager();

	// TODO implement
	// start managing the falcon pen (polling it for information)
	// create a thread to poll it
	void start() {}

	// TODO implement
	// stop managing the falcon pen
	// stop the thread that's polling it
	void stop() {}

private:
	FalconPenManager(const FalconPenManager& falconPenManager); // intentionally not implemented
	FalconPenManager& operator=(const FalconPenManager& falconPenManager); // intentionally not implemented
};

#endif // !defined(AFX_FALCONPENMANAGER_H__AFCF2776_E117_47EC_94C7_97838C3AFB59__INCLUDED_)
