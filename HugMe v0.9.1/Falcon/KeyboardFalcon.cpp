#include "KeyboardFalcon.h"
#include "WorldConstants.h"

const cCollisionAABBBox KeyboardFalcon::_boundingBox = World::local_slingshot_bounding_box;

KeyboardFalcon::KeyboardFalcon() : 
	reporting(false), 
	position(World::local_slingshot_starting_position)
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
			if (position.z - move_increment >= _boundingBox.getLowerZ())
			{
				position.z -= move_increment;
				notify(SLINGSHOT_MOVED, &position);
			}
			break;
		}
		case VK_NUMPAD5:
		{
			if (position.z + move_increment <= _boundingBox.getUpperZ())
			{
				position.z += move_increment;
				notify(SLINGSHOT_MOVED, &position);
			}
			break;
		}
		case VK_NUMPAD4:
		{
			if (position.x - move_increment >= _boundingBox.getLowerX())
			{
				position.x -= move_increment;
				notify(SLINGSHOT_MOVED, &position);
			}
			break;
		}
		case VK_NUMPAD6:
		{
			if (position.x + move_increment <= _boundingBox.getUpperX())
			{
				position.x += move_increment;
				notify(SLINGSHOT_MOVED, &position);
			}
			break;
		}
		case VK_NUMPAD7:
		{
			if (position.y + move_increment <= _boundingBox.getUpperY())
			{
				position.y += move_increment;
				notify(SLINGSHOT_MOVED, &position);
			}
			break;
		}
		case VK_NUMPAD1:
		{
			if (position.y - move_increment >= _boundingBox.getLowerY())
			{
				position.y -= move_increment;
				notify(SLINGSHOT_MOVED, &position);
			}
			break;
		}
	}
}

cCollisionAABBBox KeyboardFalcon::boundingBox() const
{
	return _boundingBox;
}