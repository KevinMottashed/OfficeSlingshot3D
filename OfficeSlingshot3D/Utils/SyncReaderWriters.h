#ifndef SYNC_READERS_WRITERS_H
#define SYNC_READERS_WRITERS_H

#include "stdafx.h"

/**
 * DEPRECATED. Utility class designed to solve the readers/writers concurrency problem.
 * In this implementation the writers are given priority. This is marked as deprecated as the boost library contains
 * something similar but more powerful/flexible so that should be used instead.
 *
 * A description of the problem as well as a pseudo code implementation can be found here:
 * http://en.wikipedia.org/wiki/Readers-writers_problem
 */
class SyncReaderWriters  
{
public:
	/**
	 * Constructor
	 */
	SyncReaderWriters();

	/**
	 * Destructor
	 */
	virtual ~SyncReaderWriters();

	/** 
	 * Lock the resource as a reader.
	 * The reader will be given access if there are no writers using or waiting for the resource.
	 */
	void lock_reader();

	/**
	 * Lock the resource as a writer.
	 * The writer will be given access when all the other readers/writers have relinquished the resource.
	 */
	void lock_writer();

	/**
	 * Unlock the resource as a reader.
	 */
	void unlock_reader();

	/**
	 * Unlock the resource as a writer.
	 */
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

/**
 * Locks a SyncReaderWriter object in reader mode following the RIAA pattern.
 * The resource is locked at contruction and released at destruction.
 */
class SyncReaderLock
{
public:
	/**
	 * Constructor. Locks the resource as a reader.
	 * @param sync The reader/writer resource to lock.
	 */
	SyncReaderLock(SyncReaderWriters& sync);

	/**
	 * Destructor. Unlocks the resource.
	 */
	~SyncReaderLock();
private:
	SyncReaderWriters* sync;
};

/**
 * Locks a SyncReaderWriter object in writer mode following the RIAA pattern.
 * The resource is locked at contruction and released at destruction.
 */
class SyncWriterLock
{
public:
	/**
	 * Constructor. Locks the resource as a writer.
	 * @param sync The reader/writer resource to lock.
	 */
	SyncWriterLock(SyncReaderWriters& sync);
	
	/**
	 * Destructor. Unlocks the resource.
	 */
	~SyncWriterLock();
private:
	SyncReaderWriters* sync;
};

#endif
 
