#ifndef SMART_CLOTHING_MANAGER_H
#define SMART_CLOTHING_MANAGER_H

#include "boost.h" // boost
#include "Configuration.h"

/**
 * Enum
 * Represents the different parts of the jacket.
 */
typedef enum tagHumanPart {
	CHEST = 0,
	RIGHT_UPPER_ARM,
	RIGHT_LOWER_ARM,
	LEFT_UPPER_ARM,
	LEFT_LOWER_ARM,
	TOTAL_NUM_PART
} HumanPart;

/**
 * Manager in charge of enabling other modules to
 * vibrate a cetain HumanPart and sets the jacket
 * and armband ports.
 */
class SmartClothingManager  
{
public:
	/**
	 * Constructor
	 * @param configuration A reference to the configuration file
	 */
	SmartClothingManager(boost::shared_ptr<Configuration> configuration);

	/**
	 * Destructor
	 */
	virtual ~SmartClothingManager();

	// implement this with a proper type for position, probably Vector3D from chai3d
	// implement this with a thread/event/timer
	// the Mediator shouldn't be blocked while the jacket vibrates (don't put a sleep)
	// if possible, make the time in ms

	/**
	 * Vibrates the smart clothing at a cetain point for a given amount of milliseconds
	 * @param touchedPart The HumainPart to be vibrated
	 * @param x The x coordinate
	 * @param y The y coordinate
	 * @param time The vibration time in milliseconds
	 */
	void vibrate(HumanPart touchedPart, double x, double y, int time);

	/**
	 * Determines where the smart clothing is to vibrate according to the position
	 * of the collision detected
	 * @param hitPart The part of the body where the hit was detected
	 * @param position The position of the ball at the moment of the collision
	 * @param minValue The hit box's minimum value
	 * @param maxValue The hit box's maximum value
	 */
	void vibrate(HumanPart hitPart, cVector3d position, cVector3d minValue, cVector3d maxValue);

	/**
	 * Set the armband and jacket ports
	 * @param armBandPort The new armband port
	 * @param jacketPort Then new jacket port
	 */
	void setPorts(int armBandPort, int jacketPort);

private:
	/**
	 * Copy-Constructor
	 */
	SmartClothingManager(const SmartClothingManager& smartClothingManager); // intentionally not implemented
	
	/**
	 * Copy-Constructor
	 */
	SmartClothingManager& operator=(const SmartClothingManager& smartClothingManager); // intentionally not implemented
};

#endif
 
