#ifndef DISPLAY_DEVICE_ARMBAND_H
#define DISPLAY_DEVICE_ARMBAND_H

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

#endif
 
