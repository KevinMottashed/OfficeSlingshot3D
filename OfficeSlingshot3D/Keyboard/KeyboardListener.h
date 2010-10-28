#ifndef KEYBOARD_LISTENER_H
#define KEYBOARD_LISTENER_H

/**
 * Interface for a class that listens to keyboard events
 */
class KeyboardListener
{
public:
	/**
	 * Constructor
	 */
	KeyboardListener();

	/**
	 * Destructor
	 */
	~KeyboardListener();

	/**
	 * Notifies the listener that a key was pressed
	 */
	virtual void keyPressed(unsigned int key) = 0;
};

#endif