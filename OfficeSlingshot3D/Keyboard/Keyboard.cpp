#include "Keyboard.h"
#include "boost.h"

using namespace std;
using namespace boost;

// singleton starts uninitialized
Keyboard* Keyboard::_instance = NULL;

Keyboard::Keyboard()
{
	// start the update thread
	updateThread = auto_ptr<thread>(new thread(bind(&Keyboard::run, this)));
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
	mutex::scoped_lock lock(queueMutex);
	queue.push(key);

	// unlock the mutex before notifying any waiting threads.
	// this allows any waiting thread to immediatly lock the mutex
	lock.unlock();
	queueEmptyCondition.notify_one();
	return;
}

void Keyboard::run()
{
	while (true)
	{
		mutex::scoped_lock lock(queueMutex);

		// wait for an update
		while (queue.empty())
		{
			queueEmptyCondition.wait(lock);
		}

		// handle the update
		unsigned int key = queue.front();
		queue.pop();

		// unlock the queue, we are done with it
		lock.unlock();

		foreach(KeyboardListener* listener, listeners)
		{
			listener->keyPressed(key);
		}
	}
}
