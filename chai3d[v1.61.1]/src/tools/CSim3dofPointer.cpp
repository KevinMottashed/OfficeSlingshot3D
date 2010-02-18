//===========================================================================
/*
    This file is part of the CHAI 3D visualization and haptics libraries.
    Copyright (C) 2003-2004 by CHAI 3D. All rights reserved.

    This library is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License("GPL") version 2
    as published by the Free Software Foundation.

    For using the CHAI 3D libraries with software that can not be combined
    with the GNU GPL, and for taking advantage of the additional benefits
    of our support services, please contact CHAI 3D about acquiring a
    Professional Edition License.

    \author:    <http://www.chai3d.org>
    \author:    Federico Barbagli
    \version    1.1
    \date       01/2004
*/
//===========================================================================

//---------------------------------------------------------------------------
#include "CSim3dofPointer.h"
#include "CMacrosGL.h"
#ifdef _WIN32
#include <process.h>
#endif

//---------------------------------------------------------------------------

//==========================================================================
/*!
      Constructor of cSim3dofPointer. 

      \fn       cSim3dofPointer::cSim3dofPointer(cWorld* a_world,
								unsigned int a_deviceNumber, bool dio_access)
      \param    a_world  World in which the tool will operate.
      \param    a_deviceNumber  0-based index used to try to open a phantom
								device.

      \param    dio_access  Also used only if the tool ends up being a phantom AND
                you're running GHOST, not OpenHaptics.  If you're not using a Phantom
                or you're using OpenHaptics, ignore this parameter.
				
                For Ghost, if dio_access is 'true', the tool will use the Ghost API's
                direct-i/o model, if it's available.  Otherwise the gstEffect i/o model
                will be used.
*/
//===========================================================================
cSim3dofPointer::cSim3dofPointer(cWorld* a_world, unsigned int a_deviceNumber, bool a_dio_access)
    :cGeneric3dofPointer(a_world)
{
    bool systemAvailable;

    m_physicalDevice = -1;


    // try to open a virtual haptic device
    m_device = new cSimulationDevice();
    systemAvailable = m_device->isSystemAvailable();

    // Is the virtual device already running?
    if (systemAvailable)
    {
        m_device->open();
        return;
    }

    // The virtual device is not running yet...
    else
    {
        // launch a virtual device
        m_device->close();
        delete m_device;
        m_device = NULL;
    }
}


//==========================================================================
/*!
      Destructor of cSim3dofPointer.

      \fn       cSim3dofPointer::~cSim3dofPointer()
*/
//===========================================================================
cSim3dofPointer::~cSim3dofPointer()
{
}
