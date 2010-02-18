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
    \author:    Force Dimension - www.forcedimension.com
    \version    1.1
    \date       01/2004
*/
//===========================================================================

//---------------------------------------------------------------------------

#include "CFingerDevices.h"

#include "CMaths.h"
#ifdef _WIN32
#include <windows.h>
#endif



//===========================================================================
/*!
    Constructor of cFingerDevice.

    \fn     cFingerDevice::cFingerDevice(unsigned int a_deviceNumber)
*/
//===========================================================================
cFingerDevice::cFingerDevice(unsigned int a_deviceNumber)
{
    // device is not yet available or ready
    m_systemAvailable = false;
    m_systemReady = false;

    m_deviceID = a_deviceNumber;

	m_systemAvailable = true;

	m_bIsZcamConnected = false;

	m_workspaceDims = 250.0;


    // init code to handle buttons
    int i = 0;
    for (i=0; i<8; i++)
    {
		m_userSwitchCount[i] = 0;
        m_userSwitchStatus[i] = 0;
        m_userSwitchClock[i].initialize();
        m_userSwitchClock[i].start();
    }
}


//===========================================================================
/*!
    Destructor of cFingerDevice.

    \fn         cFingerDevice::~cFingerDevice()
*/
//===========================================================================
cFingerDevice::~cFingerDevice()
{
    // close connection to device
    if (m_systemAvailable)
    {
		m_systemAvailable = false;
    }
}


//===========================================================================
/*!
    Open connection to delta device.

    \fn     int cFingerDevice::open()
*/
//===========================================================================
int cFingerDevice::open()
{
    // check if the system is available
    if (!m_systemAvailable) return (-1);

    // if system is already opened then return
    if (m_systemReady) return (0);

	if(!m_bIsZcamConnected) {
		m_bIsZcamConnected = initialize_DMachine_Engine();
		if(!m_bIsZcamConnected)
			return -1;
	}

	m_systemReady = true;

    return (0);
}


//===========================================================================
/*!
    Close connection to delta device.

    \fn     int cFingerDevice::close()
*/
//===========================================================================
int cFingerDevice::close()
{
    // check if the system has been opened previously
    if (!m_systemReady) return (-1);

    // update status
    m_systemReady = false;

	return 0;
}


//===========================================================================
/*!
    Calibrate delta device. 
    
    This function does nothing right now; the a_resetEncoders parameter is ignored.

    \fn     int cFingerDevice::initialize(const bool a_resetEncoders = false)
    \param  a_resetEncoders Ignored; exists for forward compatibility.
    \return Always 0
*/
//===========================================================================
int cFingerDevice::initialize(const bool a_resetEncoders)
{
    return (0);
}


//===========================================================================
/*!
    Send a command to the delta device

    \fn         int cFingerDevice::command(int a_command, void* a_data)
    \param      a_command  Selected command.
    \param      a_data  Pointer to the corresponding data structure.
    \return     Return status of command.  CHAI_MSG_OK is good, anything
                else is probably not good.
*/
//===========================================================================
int cFingerDevice::command(int a_command, void* a_data)
{
    // temp variables
    int result = CHAI_MSG_OK;
    double x=0.0,y=0.0,z=0.0;

    // check if device is open
    if (m_systemReady)
    {
        switch (a_command)
        {
            // read position of delta device
            case CHAI_CMD_GET_POS_3D:
            {
                // read position from device
				cVector3d * position = (cVector3d *) a_data;

				// Process the most recent depth camera frame
				process_zcam_data();
				// Retrieve the data from the most recent 
				get_zcam_params(hand_params.is_left_found, hand_params.is_right_found, hand_params.hand_x, 
					hand_params.hand_y, hand_params.hand_z, hand_params.fingerCount, hand_params.fingerVector);
				
				if(hand_params.is_left_found || hand_params.is_right_found)
					position->set(hand_params.hand_x, hand_params.hand_y, hand_params.hand_z);
				else
					position->set(0.0, 0.0, 0.0);

				
                // Convert from m to mm
                //position->mul(1000.0);
                //position->set(pos[1], pos[2], -pos[0]);
            }
            break;

            // read normalized position of the delta device
            case CHAI_CMD_GET_POS_NORM_3D:
            {
                // read position from device
				cVector3d * position = (cVector3d *) a_data;

				// Process the most recent depth camera frame
				process_zcam_data();
				// Retrieve the data from the most recent 
				get_zcam_params(hand_params.is_left_found, hand_params.is_right_found, hand_params.hand_x, 
					hand_params.hand_y, hand_params.hand_z, hand_params.fingerCount, hand_params.fingerVector);
				
				if(hand_params.is_left_found || hand_params.is_right_found)
					position->set(hand_params.hand_x, hand_params.hand_y, (256.0-hand_params.hand_z)*6.0);
				else
					position->set(0.0, 0.0, 0.0);

				position->div(m_workspaceDims);
            }
            break;

            // read orientation angles of delta wrist
            case CHAI_CMD_GET_ROT_ANGLES:
            {/*
                cVector3d* angles = (cVector3d *) a_data;
                angles->set(0, 0, 0);
                dhdGetOrientationRad(&angles->x, &angles->y, &angles->z, m_deviceID);
				*/
            }
            break;

            // read orientation matrix of wrist
            case CHAI_CMD_GET_ROT_MATRIX:
            {/*
                cMatrix3d frame;
                frame.identity();

                switch (m_deviceType)
                {
                    // delta devices
                    case (DHD_DEVICE_3DOF):
                    case (DHD_DEVICE_6DOF):
                    case (DHD_DEVICE_6DOF_500):
                    {
                        // read angles
                        cVector3d angles;
                        angles.set(0,0,0);
                        dhdGetOrientationRad(&angles.x, &angles.y, &angles.z, m_deviceID);

                        // compute rotation matrix
                        angles.mul(1.5);
                        frame.rotate(cVector3d(1,0,0), angles.x);
                        frame.rotate(cVector3d(0,1,0), angles.y);
                        frame.rotate(cVector3d(0,0,1), angles.z);
                    }
                    break;

                    // omega devices
                    case (DHD_DEVICE_OMEGA):
                    case (DHD_DEVICE_OMEGA3):
                    case (DHD_DEVICE_OMEGA33):
                    case (DHD_DEVICE_OMEGA331):
                    {
                        // read rotation matrix
                        double rot[3][3];
                        rot[0][0] = 1.0; rot[0][1] = 0.0; rot[0][2] = 0.0;
                        rot[1][0] = 0.0; rot[1][1] = 1.0; rot[1][2] = 0.0;
                        rot[2][0] = 0.0; rot[2][1] = 0.0; rot[2][2] = 1.0;

                        dhdGetOrientationFrame(rot, m_deviceID);

                        cMatrix3d result;
                        frame.m[0][0] = rot[0][0];
                        frame.m[0][1] = rot[0][1];
                        frame.m[0][2] = rot[0][2];
                        frame.m[1][0] = rot[1][0];
                        frame.m[1][1] = rot[1][1];
                        frame.m[1][2] = rot[1][2];
                        frame.m[2][0] = rot[2][0];
                        frame.m[2][1] = rot[2][1];
                        frame.m[2][2] = rot[2][2];
                    }
                    break;
                }

                // return result
                cMatrix3d* matrix = (cMatrix3d *) a_data;
                *matrix = frame;
*/
            }
            break;

            // set force to device
            case CHAI_CMD_SET_FORCE_3D:
            {
            }
            break;

            // set torque to device
            case CHAI_CMD_SET_TORQUE_3D:
            {/*
                cVector3d* torque = (cVector3d *) a_data;
                dhdSetTorque(torque->x, torque->y, torque->z, m_deviceID);*/
            }
            break;

            // read user switch from wrist
            case CHAI_CMD_GET_SWITCH_0:
            {/*
                int* result = (int *) a_data;
                *result = getUserSwitch(m_deviceID);*/
            }
            break;

            // read user switch from wrist
            case CHAI_CMD_GET_SWITCH_MASK:
            {/*
                int* result = (int *) a_data;

                // Force the result to be 0 or 1, since bit 0 should carry button 0's value
                *result = getUserSwitch(m_deviceID) ? 1 : 0;*/
            }
            break;

            // read scale factor from normalized coords to mm
            case CHAI_CMD_GET_NORMALIZED_SCALE_FACTOR:
            {/*
                double* scale = (double*)a_data;

                // Multiply by m_halfSizeWorkspace to get meters back
                *scale = m_halfSizeWorkspace;

                // Then multiply by 1000 to get millimeters
                *scale *= 1000.0;

                result = CHAI_MSG_OK;*/
            }
            break;

            // function is not implemented for delta devices
            default:
                result = CHAI_MSG_NOT_IMPLEMENTED;
        }
    }
    else
    {
        result = CHAI_MSG_SYSTEM_NOT_READY;
    }
    return (result);
}

//===========================================================================
/*!
    Read the user switch of the end-effector
    This function implements a small filter to avoid reading glitches.

    \fn         int cFingerDevice::getUserSwitch(int a_deviceID)
    \param      a_deviceID  device ID.
*/
//===========================================================================
int cFingerDevice::getUserSwitch(int a_deviceID)
{
	/*
    const long SWITCHTIMEOUT = 10000; // [us]

    // check device id.
    if ((a_deviceID < 0) || (a_deviceID > 7))
    {
        return (0);
    }

    // check time
    if (m_userSwitchClock[a_deviceID].getCurrentTime() < SWITCHTIMEOUT)
    {
        return (m_userSwitchStatus[a_deviceID]);
    }
    else
    {
        // timeout has occured, we read the status again
        int switchStatus;
        switchStatus = dhdGetButton(0, (char)a_deviceID);

        // if value equals to zero, check once more time
        if (switchStatus == 1)
        {
            m_userSwitchStatus[a_deviceID] = 1;
			m_userSwitchCount[a_deviceID] = 0;
        }
		else
		{
			m_userSwitchCount[a_deviceID]++;
			if (m_userSwitchCount[a_deviceID] > 6)
			{
				m_userSwitchStatus[a_deviceID] = 0;
				m_userSwitchCount[a_deviceID] = 0;			
			}
		}

        m_userSwitchClock[a_deviceID].initialize();
        m_userSwitchClock[a_deviceID].start();

        return (m_userSwitchStatus[a_deviceID]);
    }
	*/
	return 0;
}


