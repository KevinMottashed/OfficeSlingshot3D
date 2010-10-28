#ifndef DISPLAY_DEVICE_ARMBAND_H
#define DISPLAY_DEVICE_ARMBAND_H

#include "TactileArray.h"

#define ARMBAND_ARRAY_SIZE_X 3
#define ARMBAND_ARRAY_SIZE_Y 4

/**
 * Represents the Armband device and enables us
 * to activate the armbands actuators.
 */
class DisplayDeviceArmband : public TactileArray
{
public:
	/**
	 * Constructor
	 * @param a_portNum The armband's port number
	 */
	DisplayDeviceArmband(int a_portNum);

	/**
	 * Destructor
	 */
	virtual ~DisplayDeviceArmband();

	/**
	 * Resets the armband's actuators
	 */
	void setActReset()
	{
		setIntensityAll(0);
	}

	/**
	 * Actuates the armband at the given coordinates
	 * @param a_actX The x value
	 * @param a_actY The y value
	 */
	void setActForearm(double a_actX, double a_actY);

private:
	/**
	 * Array of all actuators of the armband
	 */
	int m_array[ARMBAND_ARRAY_SIZE_X][ARMBAND_ARRAY_SIZE_Y];
};

#endif
 
