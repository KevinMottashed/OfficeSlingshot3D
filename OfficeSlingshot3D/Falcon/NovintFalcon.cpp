#include "NovintFalcon.h"
#include "devices/CHapticDeviceHandler.h"

//---------------------------------------------------------------------------

#include "stdafx.h"

// cos(45 degrees)
static const float COSINE_45DEG = 0.707106;

using namespace std;
using namespace boost;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

NovintFalcon::NovintFalcon(boost::shared_ptr<Configuration> configuration) : 
	numHapticDevices(0),
	firing(false)
{
    // read the number of haptic devices currently connected to the computer
    numHapticDevices = handler.getNumDevices();

    for (int i = 0; i < numHapticDevices; ++i)
    {
        // get a handle to the next haptic device
        cGenericHapticDevice* newHapticDevice;
        handler.getDevice(newHapticDevice, i);

        // open connection to haptic device
        newHapticDevice->open();

		// initialize haptic device
		newHapticDevice->initialize();

        // store the handle in the haptic device table
        hapticDevices[i] = newHapticDevice;

        // retrieve information about the current haptic device
        cHapticDeviceInfo info = newHapticDevice->getSpecifications();

		/**
		 * Local slingshot bounding box
		 */
		double cube_side = info.m_workspaceRadius;
		//* COSINE_45DEG;

		falconBox = cCollisionAABBBox(
			cVector3d(-cube_side, -cube_side, -cube_side), 
			cVector3d(cube_side, cube_side, cube_side));
    }

	UserPreferences prefs = configuration->getUserPreferences();
	forceEnabled = prefs.forceEnabled;
}

NovintFalcon::~NovintFalcon()
{
	stopPolling();
}

void NovintFalcon::poll()
{
    // main haptic simulation loop
    while(true)
    {
        // for each device
        for (int i = 0; i < numHapticDevices; ++i)
        {
            // read position of haptic device
            cVector3d newPosition;
            hapticDevices[i]->getPosition(newPosition);

			// read linear velocity from device
            cVector3d linearVelocity;
            hapticDevices[i]->getLinearVelocity(linearVelocity);

			// CURRENTLY CAUSES FALCON TO GO CRAZY
			// compute a reaction force
            cVector3d newForce (0,0,0);

            // read user button status
            bool buttonStatus;
            hapticDevices[i]->getUserSwitch(0, buttonStatus);

			// If we want to make use of the buttom pressing
            // the user switch (ON = TRUE / OFF = FALSE)
            if (buttonStatus)
            {
				if (!firing)
				{
					firing = true;
					
				}
				
				notify(SLINGSHOT_PULLBACK, &newPosition);
				if (forceEnabled)
				{
					cHapticDeviceInfo info = hapticDevices[i]->getSpecifications();
					double Kv = info.m_maxLinearDamping;
					cVector3d force = cMul(-100.0, newPosition);
					force.x = (-info.m_workspaceRadius - newPosition.x) * 200;
					newForce.add(force);
				}
            }
			else
			{
				if (firing)
				{
					firing = false;
					notify(SLINGSHOT_FIRED, &newPosition);
				}
			}
			if (forceEnabled) {
				// send computed force to haptic device
				hapticDevices[i]->setForce(newForce);
			}

        }

		// sleep for a while so we don't hog cpu (interruption point)
		boost::this_thread::sleep(boost::posix_time::milliseconds(100));
    }
}

cCollisionAABBBox NovintFalcon::boundingBox() const
{
	return falconBox;
}

// start managing the falcon pen (polling it for information)
// create a thread to poll it
void NovintFalcon::startPolling() 
{
	if (pollingThread.get())
	{
		// the thread is already running, don't try restarting it
		return;
	}

	// create the thread
	pollingThread = auto_ptr<thread>(new thread(boost::bind(&NovintFalcon::poll, this)));

	return;
}

// stop managing the novint falcon
// stop the thread that's polling it
void NovintFalcon::stopPolling() 
{
	// make sure it's running
	if (pollingThread.get())
	{
		// interrupt and delete the thread
		pollingThread->interrupt();
		pollingThread->join();
		pollingThread.reset();
	}

	// set the force on each device to zero
	for (int i = 0; i < numHapticDevices; ++i)
    {
		hapticDevices[i]->setForce(cVector3d(0,0,0));
	}

	firing = false;


	return;
}



