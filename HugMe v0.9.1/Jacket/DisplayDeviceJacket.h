#ifndef DISPLAY_DEVICE_JACKET
#define DISPLAY_DEVICE_JACKET

#include "TactileArray.h"

#define CHEST_ARRAY_SIZE_X 6
#define CHEST_ARRAY_SIZE_Y 3
#define ARM_ARRAY_SIZE_X 4
#define ARM_ARRAY_SIZE_Y 4
#define NECK_ARRAY_SIZE_X 4
#define NECK_ARRAY_SIZE_Y 1

/**
 * Represents the Jacket device and enables us
 * to activate the jacket actuators.
 */

class DisplayDeviceJacket : public TactileArray
{
public:
	/**
	 * Constructor
	 * @param a_portNum The jacket's port number
	 */
	DisplayDeviceJacket(int a_portNum);

	/**
	 * Destructor
	 */
	virtual ~DisplayDeviceJacket();

	/**
	 * Resets the jacket's actuators
	 */
	void setActReset()
	{
		setIntensityAll(0);
	}

	/**
	 * Actuates the chest actuators at the given coordinates
	 * @param a_actX The x value
	 * @param a_actY The y value
	 */
	void setActChest(double a_actX, double a_actY);

	/**
	 * Actuates the upper arm actuators at the given coordinates
	 * @param a_actX The x value
	 * @param a_actY The y value
	 */
	void setActUpperArm(double a_actX, double a_actY);

	/**
	 * Actuates the neck actuators at the given coordinates
	 * @param a_actX The x value
	 * @param a_actY The y value
	 */
	void setActNeck(int a_actX, double a_intensity);

private:
	/**
	 * Array of all chest actuators of the jacket
	 */
	int m_chestArray[CHEST_ARRAY_SIZE_X][CHEST_ARRAY_SIZE_Y];

	/**
	 * Array of all arm actuators of the jacket
	 */
	int m_armArray[ARM_ARRAY_SIZE_X][ARM_ARRAY_SIZE_Y];

	/**
	 * Array of all neck actuators of the jacket
	 */
	int m_neckArray[NECK_ARRAY_SIZE_X][NECK_ARRAY_SIZE_Y];
};

#endif
 
