// SyncReaderWriters.cpp: implementation of the SyncReaderWriters class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SyncReaderWriters.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SyncReaderWriters::SyncReaderWriters() :
	m_readCount(0),
	m_writeCount(0)
{
	InitializeCriticalSection(&m_csReadCount);
	InitializeCriticalSection(&m_csReadRequest);
	InitializeCriticalSection(&m_csReadersAllowed);
	InitializeCriticalSection(&m_csWriteCount);
	InitializeCriticalSection(&m_csWriterAllowed);
}

SyncReaderWriters::~SyncReaderWriters()
{
	DeleteCriticalSection(&m_csReadCount);
	DeleteCriticalSection(&m_csReadRequest);
	DeleteCriticalSection(&m_csReadersAllowed);
	DeleteCriticalSection(&m_csWriteCount);
	DeleteCriticalSection(&m_csWriterAllowed);
}

void SyncReaderWriters::lock_reader()
{
	// only 1 reader can request access to the resource at a time
	EnterCriticalSection(&m_csReadRequest);
	{
		// wait until the readers have access to the resource
		EnterCriticalSection(&m_csReadersAllowed);
		{
			// only allow 1 reader to increment the counter at a time
			EnterCriticalSection(&m_csReadCount);
			{
				++m_readCount;
				if (m_readCount == 1)
				{
					// the first reader to be granted access to the resource will lock out the writers
					EnterCriticalSection(&m_csWriterAllowed);
				}
			}
			LeaveCriticalSection(&m_csReadCount);
		}
		LeaveCriticalSection(&m_csReadersAllowed);
	}
	LeaveCriticalSection(&m_csReadRequest);
	return;
}

void SyncReaderWriters::unlock_reader()
{
	// only allow 1 reader to decrement the counter at a time
	EnterCriticalSection(&m_csReadCount);
	{
		--m_readCount;
		if (m_readCount == 0)
		{
			// the last reader will grant access to the writers
			LeaveCriticalSection(&m_csWriterAllowed);
		}
	}
	LeaveCriticalSection(&m_csReadCount);
	return;
}

void SyncReaderWriters::lock_writer()
{
	// only allow 1 writer to modify the counter at a time
	EnterCriticalSection(&m_csWriteCount);
	{
		++m_writeCount;
		if (m_writeCount == 1)
		{
			// the first writer will lock out the readers
			EnterCriticalSection(&m_csReadersAllowed);
		}
	}
	LeaveCriticalSection(&m_csWriteCount);

	// only 1 writer can access the resource at a given time
	EnterCriticalSection(&m_csWriterAllowed);

	return;
}

void SyncReaderWriters::unlock_writer()
{
	// this writer is done with the resource, release it
	LeaveCriticalSection(&m_csWriterAllowed);

	// only allow 1 writer to modify the counter at a time
	EnterCriticalSection(&m_csWriteCount);
	{
		--m_writeCount;
		if (m_writeCount == 0)
		{
			// the last writer will grant access to the readers
			LeaveCriticalSection(&m_csReadersAllowed);
		}
	}
	LeaveCriticalSection(&m_csWriteCount);

	return;
}

SyncReaderLock::SyncReaderLock(SyncReaderWriters& sync) :
		sync(&sync)
{
	this->sync->lock_reader();
}

SyncReaderLock::~SyncReaderLock()
{
	sync->unlock_reader();
}

SyncWriterLock::SyncWriterLock(SyncReaderWriters& sync) :
		sync(&sync)
{
	this->sync->lock_writer();
}

SyncWriterLock::~SyncWriterLock()
{
	sync->unlock_writer();
}
