#ifndef FALCON_PEN_MANAGER_H
#define FALCON_PEN_MANAGER_H

#include "stdafx.h"
#include "FalconSubject.h"

class FalconPenManager : public FalconSubject
{
public:
	FalconPenManager();
	virtual ~FalconPenManager();

	// TODO implement
	// start managing the falcon pen (polling it for information)
	// create a thread to poll it
	void start();
	// TODO implement
	// stop managing the falcon pen
	// stop the thread that's polling it
	void stop();

	// Creates a random 3dVector
	static DWORD getPositionFromFalcon(FalconPenManager* p_Falcon);

private:
	FalconPenManager(const FalconPenManager& falconPenManager); // intentionally not implemented
	FalconPenManager& operator=(const FalconPenManager& falconPenManager); // intentionally not implemented

	bool falcon_enabled;
};

#endif

