#ifndef KEYBOARD_FALCON_H
#define KEYBOARD_FALCON_H

#include "Falcon.h"
#include "KeyboardProxy.h"
#include "chai3d.h"

/**
 * Falcon emulator that uses the keyboard instead of a novint falcon.
 * This class should mainly be used for testing purposes as we only have 1 falcon.
 */
class KeyboardFalcon : public Falcon, public KeyboardListener
{
public:
	/**
	 * Constructor
	 */
	KeyboardFalcon();

	/**
	 * Destructor
	 */
	virtual ~KeyboardFalcon();

	/**
	 * Start reporting falcon events to the mediator
	 */
	virtual void startPolling();

	/**
	 * Stop reporting falcon events to the mediator
	 */
	virtual void stopPolling();

	/**
	 * Handle a key pressed event
	 */
	virtual void keyPressed(unsigned int key);

	/**
	 * Returns the box to which the slingshot is bound.
	 * @return the box to which the slingshot is bound
	 */
	virtual cCollisionAABBBox boundingBox() const;

private:

	/**
	 * The keyboard falcon will only report events to the mediator when this is true
	 */
	bool reporting;

	/**
	 * The falcon's current position
	 */
	cVector3d position;

	/**
	 * The keyboard falcon cannot move outside of this box
	 */
	static const cCollisionAABBBox _boundingBox;
};

#endif