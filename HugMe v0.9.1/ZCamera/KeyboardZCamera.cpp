#include "KeyboardZCamera.h"
#include "WorldConstants.h"

KeyboardZCamera::KeyboardZCamera() : 
	reporting(false), 
	position(World::local_avatar_starting_position)
{
	// add ourselves as a keyboard listener
	Keyboard::instance()->attach(this);
}

KeyboardZCamera::~KeyboardZCamera()
{
	// remove ourselves as a keyboard listener
	Keyboard::instance()->detach(this);
}

void KeyboardZCamera::startCapture()
{
	reporting = true;
}

void KeyboardZCamera::stopCapture()
{
	reporting = false;
}

void KeyboardZCamera::keyPressed(unsigned int key)
{
	// make sure mediator wants the input (game started)
	if (!reporting)
	{
		return;
	}

	static const float move_increment = 6.0f;
	static const float head_position = 120.0f;

	switch (key)
	{
		case VK_LEFT:
		{
			position.x -= move_increment;
			position.y = 120.0f;
			notify(AVATAR_POSITION, &position);
			break;
		}
		case VK_RIGHT:
		{
			position.x += move_increment;
			position.y = 120.0f;
			notify(AVATAR_POSITION, &position);
			break;
		}
	}
}