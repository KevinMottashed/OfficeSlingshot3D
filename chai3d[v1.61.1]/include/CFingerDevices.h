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
    \author:    Francois Conti
    \version    1.1
    \date       01/2004
*/
//===========================================================================

//---------------------------------------------------------------------------
#ifndef CFingerDevicesH
#define CFingerDevicesH

//---------------------------------------------------------------------------
#include "CGenericDevice.h"
#include "CPrecisionClock.h"
#include "ZCAM_HEADER1.h"

typedef struct HAND_REP
{
	int is_left_found;
	int is_right_found;
	float hand_x;
	float hand_y;
	float hand_z;
	
	float fingerVector;
	int fingerCount;
}HandParams;

//===========================================================================
/*!
    \class cFingerDevice
    \brief
    Interface to delta and omega devices
*/
//===========================================================================
class cFingerDevice : public cGenericDevice
{
  public:
    // CONSTRUCTOR & DESTRUCTOR:
    //! Constructor of cFingerDevices.
    cFingerDevice(unsigned int a_deviceNumber);

    //! Destructor of cGenericDevice.
    virtual ~cFingerDevice();

    // METHODS:
    //! Open connection to delta device.
    virtual int open();

    //! Close connection to delta device
    virtual int close();

    //! Calibrate delta device.
    virtual int initialize(const bool a_resetEncoders=false);

    //! Set a command to the delta device
    virtual int command(int a_command, void* a_data);

	HandParams hand_params;

  protected:

	  bool m_bIsZcamConnected;

    //! Reference count used to control access to the dhd dll
    static int m_activeFingerDevices;

    //! Device ID number
    int m_deviceID;

    // Device workspace dimensions
    double m_workspaceDims;

    //! Maximum forces
    double m_maximumForces;

    //! Last position of user switch
    int m_userSwitchCount[8];
	int m_userSwitchStatus[8];
    cPrecisionClock m_userSwitchClock[8];

    //! Read user switch from end-effector
    int getUserSwitch(int a_deviceID);
};

//---------------------------------------------------------------------------
#endif
//---------------------------------------------------------------------------

