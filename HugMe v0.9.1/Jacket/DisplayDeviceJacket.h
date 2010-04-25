#ifndef DISPLAY_DEVICE_JACKET
#define DISPLAY_DEVICE_JACKET

#include "TactileArray.h"

#define CHEST_ARRAY_SIZE_X 6
#define CHEST_ARRAY_SIZE_Y 3
#define ARM_ARRAY_SIZE_X 4
#define ARM_ARRAY_SIZE_Y 4
#define NECK_ARRAY_SIZE_X 4
#define NECK_ARRAY_SIZE_Y 1

class DisplayDeviceJacket : public TactileArray
{
public:
	DisplayDeviceJacket(int a_portNum);
	virtual ~DisplayDeviceJacket();

	int m_chestArray[CHEST_ARRAY_SIZE_X][CHEST_ARRAY_SIZE_Y];
	int m_armArray[ARM_ARRAY_SIZE_X][ARM_ARRAY_SIZE_Y];
	int m_neckArray[NECK_ARRAY_SIZE_X][NECK_ARRAY_SIZE_Y];

	void setActReset()
	{
		setIntensityAll(0);
	}
	void setActChest(double a_actX, double a_actY);
	void setActUpperArm(double a_actX, double a_actY);
	void setActNeck(int a_actX, double a_intensity);
};

#endif
 
