#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "stdafx.h"
#include "boost.h"
#include "KeyboardListener.h"

/**
 * Singleton class that represents the keyboard
 * The active window should monitor keystrokes and report them here.
 * Only the active window should monitor keystrokes otherwise we won't be
 * able to test 2 instances on 1 pc.
 */
class Keyboard
{
public:
	/**
	 * Kill me now
	 */
	~Keyboard();

	/**
	 * Retrieve the singleton
	 * @return the singleton
	 */
	static Keyboard* instance();

	/**
	 * Attach a listener to the keyboard
	 * @param listener The listener to attach
	 */
	void attach(KeyboardListener* listener);

	/**
	 * Detach a listener from the keyboard
	 * @param listener The listener to detach
	 */
	void detach(KeyboardListener* listener);

	/**
	 * Let all the listeners know that a key was pressed
	 * @param key The key that was pressed
	 */
	void keyPressed(unsigned int key);

private:
	// all private for singleton
	Keyboard();
	Keyboard(const Keyboard& other);
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