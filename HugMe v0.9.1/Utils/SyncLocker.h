#ifndef SYNC_LOCKER_H
#define SYNC_LOCKER_H

#include "stdafx.h"

class SyncLocker
{
	CRITICAL_SECTION* cs;
public:
	SyncLocker(CRITICAL_SECTION& cs)
	{
		this->cs = &cs;
		EnterCriticalSection(this->cs);
	}
	~SyncLocker()
	{
		LeaveCriticalSection(this->cs);
	}
	operator bool()
	{
		return false;
	}
};

#define synchronized(obj) if (SyncLocker __sync_locker__ = obj) ASSERT(0); else

#endif

