#ifndef KEYBOARD_ZCAMERA_H
#define KEYBOARD_ZCAMERA_H

#include "ZCamera.h"
#include "KeyboardProxy.h"
#include "chai3d.h"

/**
 * ZCamera emulator that uses the keyboard instead of a z-camera.
 */
class KeyboardZCamera : public IZCamera, public KeyboardListener
{
public:
	/**
	 * Constructor
	 */
	KeyboardZCamera();

	/**
	 * Destructor
	 */
	virtual ~KeyboardZCamera();

	/**
	 * Start reporting camera events to the mediator
	 */
	virtual void startCapture();

	/**
	 * Stop reporting camera events to the mediator
	 */
	virtual void stopCapture();

	/**
	 * Handle a key pressed event
	 */
	virtual void keyPressed(unsigned int key);

private:

	/**
	 * The keyboard zcamera will only report events to the mediator when this is true
	 */
	bool reporting;

	/**
	 * The avatar's current position
	 */
	cVector3d position;
};

#endif