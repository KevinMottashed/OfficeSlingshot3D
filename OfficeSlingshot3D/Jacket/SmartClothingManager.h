#ifndef SMART_CLOTHING_MANAGER_H
#define SMART_CLOTHING_MANAGER_H

#include "boost.h" // boost
#include "Configuration.h"
#include "BodyPartEnum.h"

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


	/**
	 * Vibrates the smart clothing at a cetain point for a 1000 milliseconds
	 * @param touchedPart The body part to be vibrated
	 * @param x The x coordinate
	 * @param y The y coordinate
	 */
	void vibrate(BodyPart_t touchedPart, double x, double y);

	/**
	 * Determines where the smart clothing is to vibrate according to the position
	 * of the collision detected
	 * @param hitPart The part of the body where the hit was detected
	 * @param position The position of the ball at the moment of the collision
	 * @param minValue The hit box's minimum value
	 * @param maxValue The hit box's maximum value
	 */
	void vibrate(BodyPart_t hitPart, cVector3d position, cVector3d minValue, cVector3d maxValue);

	/**
	 * Set the armband and jacket ports
	 * @param armBandPort The new armband port
	 * @param jacketPort Then new jacket port
	 */
	void setPorts(int armBandPort, int jacketPort);

private:
	
	/**
	 * Stops the vibration after 1000 ms
	 */
	void TimerProc();

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
 
