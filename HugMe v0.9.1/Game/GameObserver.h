#ifndef GAME_OBSERVER_H
#define GAME_OBSERVER_H

#include "stdafx.h"

// each update has a context
// the context represents what has changed in the observed object
enum GameUpdateContext
{
};

class GameObserver
{
public:
	GameObserver();
	virtual ~GameObserver();

	// handles an update
	virtual void update(GameUpdateContext context, const void* data) = 0;
};

#endif