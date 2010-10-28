#include "FalconReplayer.h"

FalconReplayer::FalconReplayer(	boost::shared_ptr<std::ifstream> file,
								boost::shared_ptr<boost::archive::text_iarchive> archive) :
	file(file),
	archive(archive)
{
}

FalconReplayer::~FalconReplayer()
{
	// The archive must be destroyed before the file stream.
	// This is because the archive uses the file so if we destroy
	// the file first the archive will no longer have a file.
	archive.reset();
	file.reset();
}

void FalconReplayer::replay(LogEvent_t logEvent)
{
	// don't try handling an event that isnt a falcon event
	assert(LogEventCategory::lookupCategory(logEvent) == LogEventCategory::FALCON);

	switch (logEvent)
	{
		case LogEvent::FALCON_SLINGSHOT_POSITION:
		{
			cVector3d vec;
			*archive >> vec;

			notify(SLINGSHOT_MOVED, &vec);
			break;
		}
		case LogEvent::FALCON_SLINGSHOT_FIRED:
		{
			notify(SLINGSHOT_FIRED);
			break;
		}
	}
		
	return;
}

void FalconReplayer::startPolling()
{
	return;
}

void FalconReplayer::stopPolling()
{
	return;
}

cCollisionAABBBox FalconReplayer::boundingBox() const
{
	cCollisionAABBBox temp;
	return temp;
}

