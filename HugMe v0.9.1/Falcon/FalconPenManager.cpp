// FalconPenManager.cpp: implementation of the FalconPenManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FalconPenManager.h"

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
	//cVector3d position;
	
	int projectileCount = 0;

	while(p_Falcon->falcon_enabled) 
	{
		x = rand() % 10;
		y = rand() % 10;
		z = rand() % 10;
		
		cVector3d position;

		position.x = x;
		position.y = y;
		position.z = z;

		Controller::instance()->notifyNewLocalSlingshotPosition(position);


		if (projectileCount < 5)
		{
			++projectileCount;

			Projectile projectile;

			projectile.setSpeed(x, y, z);

			projectile.setPosition(x, y, z);

			Controller::instance()->notifyNewLocalProjectile(projectile);
		}

		// sleep for 100ms
		Sleep(100);
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

