#include "stdafx.h"
#include "FalconPenManager.h"
#include "chai3d.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

FalconPenManager::FalconPenManager()
{
	falcon_enabled = false;
}

FalconPenManager::~FalconPenManager()
{

}

DWORD FalconPenManager::getPositionFromFalcon(FalconPenManager* p_Falcon)
{
	double x,y,z;

	while(p_Falcon->falcon_enabled) 
	{
		x = rand() % 10;
		y = rand() % 10;
		z = rand() % 10;
		
		cVector3d position;

		position.x = x;
		position.y = y;
		position.z = z;

		p_Falcon->notify(SLINGSHOT_POSITION, &position);		

		// sleep for 1000ms
		Sleep(1000);
	}

	return 0;

}

// TODO implement
// start managing the falcon pen (polling it for information)
// create a thread to poll it
void FalconPenManager::start() 
{
	falcon_enabled = true;

	// Thread Generation, modified from ZCameraManager.cpp
	DWORD threadId;
	HANDLE hThread = CreateThread( 0, 0,(LPTHREAD_START_ROUTINE) getPositionFromFalcon,  (void*) this, 0, &threadId);

}

// TODO implement
// stop managing the falcon pen
// stop the thread that's polling it
void FalconPenManager::stop() 
{
	falcon_enabled = false;

	// Kill thread here
}

