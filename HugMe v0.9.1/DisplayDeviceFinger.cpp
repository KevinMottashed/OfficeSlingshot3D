// DisplayDeviceFinger.cpp: implementation of the DisplayDeviceFinger class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Chat.h"
#include "DisplayDeviceFinger.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

DisplayDeviceFinger::DisplayDeviceFinger(int a_portNum) : TactileArray(a_portNum)
{
	int i, j;
	for(i=0; i<FINGER_ARRAY_SIZE_X; i++)
		for(j=0; j<FINGER_ARRAY_SIZE_Y; j++)
			m_array[i][j] = j*FINGER_ARRAY_SIZE_X + i;
	
	int totalSize = FINGER_ARRAY_SIZE_X*FINGER_ARRAY_SIZE_Y;

	// Need to change the protocol between the PC and the microcontroller.
	// Since the last two values have problem, I just added two more values and just used all without the last two.
	setArraySize(1, totalSize+11);
}

DisplayDeviceFinger::~DisplayDeviceFinger()
{

}

void DisplayDeviceFinger::setActFinger(double a_actY)
{
	double coordY = a_actY * (double)(FINGER_ARRAY_SIZE_Y-1);
	double coordYF = floor(coordY);

	setIntensityAll(0);

	if(coordY == coordYF) {
		int actIndex, actInt;
		actIndex = m_array[0][(int)coordY];
		actInt = (double)MAX_INTENSITY;
		setIntensity(0, actIndex, actInt);
	} else {
	int actIndex[2], actInt[2];
		actIndex[0] = m_array[0][(int)coordYF];
		actInt[0] = (int)floor( (1.0 + coordYF - coordY)*(double)MAX_INTENSITY );
		actIndex[1] = m_array[0][(int)coordYF+1];
		actInt[1] = (int)floor( (coordY - coordYF)*(double)MAX_INTENSITY );
		setIntensity(0, actIndex[0], actInt[0]);
		setIntensity(0, actIndex[1], actInt[1]);
	}
}