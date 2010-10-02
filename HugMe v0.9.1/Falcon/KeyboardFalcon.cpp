#include "KeyboardFalcon.h"

KeyboardFalcon::KeyboardFalcon() : 
	reporting(false), 
	position(0,0,0)
{
	// add ourselves as a keyboard listener
	Keyboard::instance()->attach(this);
}

KeyboardFalcon::~KeyboardFalcon()
{
	// remove ourselves as a keyboard listener
	Keyboard::instance()->detach(this);
}

void KeyboardFalcon::startPolling()
{
	reporting = true;
}

void KeyboardFalcon::stopPolling()
{
	reporting = false;
}

void KeyboardFalcon::keyPressed(unsigned int key)
{
	// make sure mediator wants the input (game started)
	if (!reporting)
	{
		return;
	}

	static const float move_increment = 0.1f;

	switch (key)
	{
		case VK_SPACE:
		{
			notify(SLINGSHOT_FIRED);
			break;
		}
		case VK_NUMPAD8:
		{
			position.x -= move_increment;
			notify(SLINGSHOT_MOVED, &position);
			break;
		}
		case VK_NUMPAD5:
		{
			position.x += move_increment;
			notify(SLINGSHOT_MOVED, &position);
			break;
		}
		case VK_NUMPAD4:
		{
			position.y -= move_increment;
			notify(SLINGSHOT_MOVED, &position);
			break;
		}
		case VK_NUMPAD6:
		{
			position.y += move_increment;
			notify(SLINGSHOT_MOVED, &position);
			break;
		}
		case VK_NUMPAD7:
		{
			position.z += move_increment;
			notify(SLINGSHOT_MOVED, &position);
			break;
		}
		case VK_NUMPAD1:
		{
			position.z -= move_increment;
			notify(SLINGSHOT_MOVED, &position);
			break;
		}
	}
}