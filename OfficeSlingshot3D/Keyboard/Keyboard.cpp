#include "Keyboard.h"
#include "boost.h"

// singleton starts uninitialized
Keyboard* Keyboard::_instance = NULL;

Keyboard::Keyboard()
{
}

Keyboard::~Keyboard()
{
}

Keyboard* Keyboard::instance()
{
	if (_instance == NULL)
	{
		_instance = new Keyboard();
	}
	return _instance;
}

void Keyboard::attach(KeyboardListener* listener)
{
	// no duplicates
	assert(std::find(listeners.begin(), listeners.end(), listener) == listeners.end());
	listeners.push_back(listener);
	return;
}

void Keyboard::detach(KeyboardListener* listener)
{
	listeners.erase(std::remove(listeners.begin(), listeners.end(), listener), listeners.end());
	return;
}

void Keyboard::keyPressed(unsigned int key)
{
	foreach(KeyboardListener* listener, listeners)
	{
		listener->keyPressed(key);
	}
	return;
}