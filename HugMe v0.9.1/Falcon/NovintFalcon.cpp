#include "NovintFalcon.h"
#include "devices/CHapticDeviceHandler.h"
#include "WorldConstants.h"

//---------------------------------------------------------------------------

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// maximum number of haptic devices supported in this demo
const int MAX_DEVICES           = 4;

// number of haptic devices detected
int numHapticDevices = 0;

// root resource path
string resourceRoot;

// a table containing pointers to all haptic devices detected on this computer
cGenericHapticDevice* hapticDevices[MAX_DEVICES];

NovintFalcon* p_Falcon;

void updateHaptics(void);

//---------------------------------------------------------------------------
// DECLARED MACROS
//---------------------------------------------------------------------------
// convert to resource path
#define RESOURCE_PATH(p)    (char*)((resourceRoot+string(p)).c_str())


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

NovintFalcon::NovintFalcon() : 
	reporting(false),
	position(World::local_slingshot_starting_position)
{

	resourceRoot = "C:\\cygwin\\home\\Administrator\\HugMe\\HugMe v0.9.1\\Debug";

	// a haptic device handler
	cHapticDeviceHandler* handler;

	// create a haptic device handler
	handler = new cHapticDeviceHandler();

    // read the number of haptic devices currently connected to the computer
    numHapticDevices = handler->getNumDevices();

	int i = 0;
    while (i < numHapticDevices)
    {
        // get a handle to the next haptic device
        cGenericHapticDevice* newHapticDevice;
        handler->getDevice(newHapticDevice, i);

        // open connection to haptic device
        newHapticDevice->open();

		// initialize haptic device
		newHapticDevice->initialize();

        // store the handle in the haptic device table
        hapticDevices[i] = newHapticDevice;

        // retrieve information about the current haptic device
        cHapticDeviceInfo info = newHapticDevice->getSpecifications();

        // create a string that concatenates the device number and model name.
        string strID;
        cStr(strID, i);
        string strDevice = "#" + strID + " - " +info.m_modelName;

		printf("%s\n", strDevice);

		

		// increment counter
        i++;
    }

	p_Falcon = this;
}

NovintFalcon::~NovintFalcon()
{

}

void updateHaptics(void)
{
    // main haptic simulation loop
    while(true)
    {
        // for each device
        int i=0;
        while (i < numHapticDevices)
        {
            // read position of haptic device
            cVector3d newPosition;
            hapticDevices[i]->getPosition(newPosition);
			p_Falcon->notify(SLINGSHOT_MOVED, &newPosition);

			// read linear velocity from device
            cVector3d linearVelocity;
            hapticDevices[i]->getLinearVelocity(linearVelocity);

			// compute a reaction force
			cVector3d newForce (0,0,0);

			// apply force field
			if (true)
			{
			   double Kp = 20.0; // [N/m]
			   cVector3d force = cMul(-Kp, newPosition);
			   newForce.add(force);
			}

			// send computed force to haptic device
            hapticDevices[i]->setForce(newForce);

            // read user button status
            bool buttonStatus;
            hapticDevices[i]->getUserSwitch(0, buttonStatus);

			// If we want to make use of the buttom pressing
            // the user switch (ON = TRUE / OFF = FALSE)
            if (buttonStatus)
            {
                p_Falcon->notify(SLINGSHOT_FIRED);
            }
            else
            {
                
            }

            // increment counter
            i++;
        }
    }

	

}

cCollisionAABBBox NovintFalcon::boundingBox() const
{
	// TODO: Alex, add whatever area the novint falcon is bound to here
	cCollisionAABBBox temp;
	return temp;
}

// TODO implement
// start managing the falcon pen (polling it for information)
// create a thread to poll it
void NovintFalcon::startPolling() 
{
	reporting = true;

	// create a thread which starts the main haptics rendering loop
    cThread* hapticsThread = new cThread();
	hapticsThread->set(updateHaptics, CHAI_THREAD_PRIORITY_HAPTICS);

}

// TODO implement
// stop managing the falcon pen
// stop the thread that's polling it
void NovintFalcon::stopPolling() 
{
	reporting = false;

	// Kill thread here
}



