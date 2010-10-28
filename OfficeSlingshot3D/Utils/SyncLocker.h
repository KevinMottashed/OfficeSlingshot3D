#ifndef SYNC_LOCKER_H
#define SYNC_LOCKER_H

#include "stdafx.h"

/**
 * Utility class for wrapping a mutex in the RIAA pattern.
 * The mutex is locked on construction and released upon destruction.
 */
class SyncLocker
{
public:
	/**
	 * Constructor. Locks the mutex.
	 * @param cs The mutex to lock.
	 */
	SyncLocker(CRITICAL_SECTION& cs)
	{
		this->cs = &cs;
		EnterCriticalSection(this->cs);
	}

	/**
	 * Destructor. Releases the mutex.
	 */
	~SyncLocker()
	{
		LeaveCriticalSection(this->cs);
	}

	/**
	 * Typecast to bool operator.
	 * Provided to aid the synchronized macro.
	 * @return false
	 */
	operator bool()
	{
		return false;
	}
private:
	CRITICAL_SECTION* cs;
};


#define synchronized(obj) if (SyncLocker __sync_locker__ = obj) ASSERT(0); else

#endif

