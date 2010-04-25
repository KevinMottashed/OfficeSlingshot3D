#ifndef SYNC_READERS_WRITERS_H
#define SYNC_READERS_WRITERS_H

#include "stdafx.h"

/*
 * The SyncReaderWriters is a utility class designed to solve the readers/writers concurrency problem
 * In this implementation the writers are given priority.
 *
 * A description of the problem as well as a pseudo code implementation can be found here:
 * http://en.wikipedia.org/wiki/Readers-writers_problem
 *
 */

class SyncReaderWriters  
{
public:
	SyncReaderWriters();
	virtual ~SyncReaderWriters();

	// A reader is requesting access to the resource
	// The reader will be given access if there are no writers using or waiting for the resource
	void lock_reader();

	// A writer is requesting access to the resource
	// The writer will be given access when all the other readers/writers have relinquished the resource
	void lock_writer();

	// A reader is relinquishing access to the resource
	void unlock_reader();

	// A writer is relinquishing access to the resource
	void unlock_writer();

private:
	// not implemented on purpose
	SyncReaderWriters(const SyncReaderWriters& sync);
	SyncReaderWriters& operator=(const SyncReaderWriters& sync);

	// keeps track of how many readers are currently reading the resource
	int m_readCount;

	// keeps track of how many writers are waiting for the resource
	int m_writeCount;

	// protects m_readCount
	CRITICAL_SECTION m_csReadCount;

	// allows only 1 reader to request access at a time
	CRITICAL_SECTION m_csReadRequest;

	// used to lock out the readers when there are writers present
	CRITICAL_SECTION m_csReadersAllowed;

	// protects m_writeCount
	CRITICAL_SECTION m_csWriteCount;

	// used to synchronize the writers so that only one of them has access at a time
	CRITICAL_SECTION m_csWriterAllowed;
};

/*
 * The SyncReaderLock is a utility class to be used in conjunction with the SyncReaderWriters class
 * This class will lock the resource as a reader upon creation and release upon destruction
 */
class SyncReaderLock
{
public:
	SyncReaderLock(SyncReaderWriters& sync);
	~SyncReaderLock();
private:
	SyncReaderWriters* sync;
};

/*
 * The SyncWriterLock is a utility class to be used in conjunction with the SyncReaderWriters class
 * This class will lock the resource as a writer upon creation and release upon destruction
 */
class SyncWriterLock
{
public:
	SyncWriterLock(SyncReaderWriters& sync);
	~SyncWriterLock();
private:
	SyncReaderWriters* sync;
};

#endif
 
