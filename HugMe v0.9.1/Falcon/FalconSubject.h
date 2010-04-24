#ifndef FALCON_SUBJECT_H_
#define FALCON_SUBJECT_H_

#include "stdafx.h"
#include "FalconObserver.h"

class FalconSubject
{
public:
	FalconSubject();
	virtual ~FalconSubject();

	void attach(FalconObserver* observer);
	void detach(FalconObserver* observer);

	void notify(FalconUpdateContext context, const void* data = NULL);

private:
	std::vector<FalconObserver*> m_observers;
};

#endif
