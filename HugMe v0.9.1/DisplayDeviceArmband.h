// DisplayDeviceArmband.h: interface for the DisplayDeviceArmband class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DISPLAYDEVICEARMBAND_H__809A67E4_0DFE_414D_B801_86B8A624097A__INCLUDED_)
#define AFX_DISPLAYDEVICEARMBAND_H__809A67E4_0DFE_414D_B801_86B8A624097A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TactileArray.h"

#define ARMBAND_ARRAY_SIZE_X 3
#define ARMBAND_ARRAY_SIZE_Y 4

class DisplayDeviceArmband : public TactileArray
{
public:
	DisplayDeviceArmband(int a_portNum);
	virtual ~DisplayDeviceArmband();

	int m_array[ARMBAND_ARRAY_SIZE_X][ARMBAND_ARRAY_SIZE_Y];

	void setActReset()
	{
		setIntensityAll(0);
	}
	void setActForearm(double a_actX, double a_actY);
};

#endif // !defined(AFX_DISPLAYDEVICEARMBAND_H__809A67E4_0DFE_414D_B801_86B8A624097A__INCLUDED_)
