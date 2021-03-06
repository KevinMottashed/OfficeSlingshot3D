#include "KeyboardFalcon.h"
#include "WorldConstants.h"
#include "PerspectiveMath.h"



KeyboardFalcon::KeyboardFalcon() : 
	reporting(false),
	firing(false),
	position(	World::local_ball_starting_position.z, 
		World::local_ball_starting_position.x, 
		World::local_ball_starting_position.y)
{
	_boundingBox = cCollisionAABBBox(World::local_ball_bounding_box);
	PerspectiveMath::convertBoxOrientationGameToNovint(_boundingBox);
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
	firing = false;

	position = cVector3d(World::local_ball_starting_position.z, 
		World::local_ball_starting_position.x, 
		World::local_ball_starting_position.y);
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
		case VK_SPACE: // pullback/shoot
		{
			if (!firing)
			{
				firing = true;
				notify(SLINGSHOT_PULLBACK, &position);
			}
			else
			{
				firing = false;
				notify(SLINGSHOT_FIRED, &position);
				resetPosition();
			}
			break;
		}
		case VK_NUMPAD7: // move forward
		{
			if (position.x - move_increment >= _boundingBox.getLowerX())
			{
				position.x -= move_increment;
				if (firing)
				{
					notify(SLINGSHOT_PULLBACK, &position);
				}
			}
			break;
		}
		case VK_NUMPAD1: // move backwards
		{
			if (position.x + move_increment <= _boundingBox.getUpperX())
			{
				position.x += move_increment;
				if (firing)
				{
					notify(SLINGSHOT_PULLBACK, &position);
				}
			}
			break;
		}
		case VK_NUMPAD4: // move left
		{
			if (position.y - move_increment >= _boundingBox.getLowerY())
			{
				position.y -= move_increment;
				if (firing)
				{
					notify(SLINGSHOT_PULLBACK, &position);
				}
			}
			break;
		}
		case VK_NUMPAD6: // move right
		{
			if (position.y + move_increment <= _boundingBox.getUpperY())
			{
				position.y += move_increment;
				if (firing)
				{
					notify(SLINGSHOT_PULLBACK, &position);
				}
			}
			break;
		}
		case VK_NUMPAD8: // move up
		{
			if (position.z + move_increment <= _boundingBox.getUpperZ())
			{
				position.z += move_increment;
				if (firing)
				{
					notify(SLINGSHOT_PULLBACK, &position);
				}
			}
			break;
		}
		case VK_NUMPAD5: // move down
		{
			if (position.z - move_increment >= _boundingBox.getLowerZ())
			{
				position.z -= move_increment;
				if (firing)
				{
					notify(SLINGSHOT_PULLBACK, &position);
				}
			}
			break;
		}
	}
}

cCollisionAABBBox KeyboardFalcon::boundingBox() const
{
	return _boundingBox;
}

void KeyboardFalcon::resetPosition(){
	position = cVector3d(World::local_ball_starting_position.z, 
		World::local_ball_starting_position.x, 
		World::local_ball_starting_position.y);
}