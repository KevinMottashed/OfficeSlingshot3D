#ifndef GAME_SUBJECT_H_
#define GAME_SUBJECT_H_

#include "stdafx.h"
#include "GameObserver.h"

class GameSubject
{
public:
	GameSubject();
	virtual ~GameSubject();

	void attach(GameObserver* observer);
	void detach(GameObserver* observer);

	void notify(GameUpdateContext context, const void* data = NULL);

private:
	std::vector<GameObserver*> m_observers;
};

#endif

