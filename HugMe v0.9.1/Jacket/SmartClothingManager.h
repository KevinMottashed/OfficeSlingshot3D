#ifndef SMART_CLOTHING_MANAGER_H
#define SMART_CLOTHING_MANAGER_H

#include "boost.h" // boost
#include "Configuration.h"

//Should be created in HumanModel... 
typedef enum tagHumanPart {
	CHEST = 0,
	RIGHT_UPPER_ARM,
	RIGHT_LOWER_ARM,
	TOTAL_NUM_PART
} HumanPart;

class SmartClothingManager  
{
public:
	SmartClothingManager(boost::shared_ptr<Configuration> configuration);
	virtual ~SmartClothingManager();

	// implement this with a proper type for position, probably Vector3D from chai3d
	// implement this with a thread/event/timer
	// the Mediator shouldn't be blocked while the jacket vibrates (don't put a sleep)
	// if possible, make the time in ms
	void vibrate(HumanPart touchedPart, int x, int y, int time);
	void setPorts(int armBandPort, int jacketPort);

private:
	SmartClothingManager(const SmartClothingManager& smartClothingManager); // intentionally not implemented
	SmartClothingManager& operator=(const SmartClothingManager& smartClothingManager); // intentionally not implemented
};

#endif
 
