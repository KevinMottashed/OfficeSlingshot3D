#ifndef ZCAMERA_SUBJECT_H
#define ZCAMERA_SUBJECT_H

#include "stdafx.h"
#include "ZCameraObserver.h"

class ZCameraSubject
{
public:
	ZCameraSubject();
	virtual ~ZCameraSubject();

	void attach(ZCameraObserver* observer);
	void detach(ZCameraObserver* observer);

	void notify(ZCameraUpdateContext context, const void* data = NULL);

private:
	std::vector<ZCameraObserver*> m_observers;
};

#endif