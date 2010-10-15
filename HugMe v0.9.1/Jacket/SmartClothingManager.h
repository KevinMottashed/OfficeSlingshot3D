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
	void vibrate(HumanPart touchedPart, int x, int y, int time);

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
 
