#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "stdafx.h"
#include "boost.h"
#include "KeyboardListener.h"

/**
 * @ingroup Keyboard
 * @b public
 * Singleton class that represents the keyboard.
 * The active window should monitor keystrokes and report them here.
 * Only the active window should monitor keystrokes otherwise we won't be
 * able to test 2 instances on 1 pc.
 */
class Keyboard
{
public:	
	~Keyboard(); /**< Destructor. */

	/**
	 * Retrieve the singleton.
	 * @return the singleton
	 */
	static Keyboard* instance();

	/**
	 * Attach a listener to the keyboard.
	 * @param[in] listener The listener to attach.
	 */
	void attach(KeyboardListener* listener);

	/**
	 * Detach a listener from the keyboard.
	 * @param[in] listener The listener to detach.
	 */
	void detach(KeyboardListener* listener);

	/**
	 * Let all the listeners know that a key was pressed.
	 * @param[in] key The key that was pressed.
	 */
	void keyPressed(unsigned int key);

private:
	Keyboard(); /**< Constructor. Private for singleton pattern. */

	/**
	 * Copy constructor. Not implemented to protect from use.
	 * @param[in] other The object to copy.
	 */
	Keyboard(const Keyboard& other);

	/**
	 * Assignment operator. Not implemented to protect from use.
	 * @param[in] other The object to copy.
	 */
	Keyboard& operator= (const Keyboard& other);

	/**
	 * The list of listeners
	 */
	std::vector<KeyboardListener*> listeners;

	std::auto_ptr<boost::thread> updateThread; /**< Thread that handles keyboard updates. */
	std::queue<unsigned int> queue; /**< Queue containing all the pressed keys that havent been handled */
	boost::mutex queueMutex; /**< Guards the queue */
	boost::condition_variable queueEmptyCondition; /**< Condition variable for when the queue is empty */

	/**
	 * Start handling key presses.
	 */
	void run();

	/**
	 * The singleton instance
	 */
	static Keyboard* _instance;
};

#endif