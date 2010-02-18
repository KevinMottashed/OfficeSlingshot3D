// DisplayDeviceFinger.h: interface for the DisplayDeviceFinger class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DISPLAYDEVICEFINGER_H__2023312B_2262_4CED_92C0_3CF406645864__INCLUDED_)
#define AFX_DISPLAYDEVICEFINGER_H__2023312B_2262_4CED_92C0_3CF406645864__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TactileArray.h"

#define FINGER_ARRAY_SIZE_X 1
#define FINGER_ARRAY_SIZE_Y 3

class DisplayDeviceFinger : public TactileArray
{
public:
	DisplayDeviceFinger(int a_portNum);
	virtual ~DisplayDeviceFinger();

	int m_array[FINGER_ARRAY_SIZE_X][FINGER_ARRAY_SIZE_Y];

	void setActReset()
	{
		setIntensityAll(0);
	}

	void setActFinger(double a_actY);
};

#endif // !defined(AFX_DISPLAYDEVICEFINGER_H__2023312B_2262_4CED_92C0_3CF406645864__INCLUDED_)
