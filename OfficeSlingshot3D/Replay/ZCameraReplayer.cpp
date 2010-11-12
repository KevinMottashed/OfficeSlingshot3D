#include "ZCameraReplayer.h"

ZCameraReplayer::ZCameraReplayer(	boost::shared_ptr<std::ifstream> file,
									boost::shared_ptr<boost::archive::text_iarchive> archive) :
	file(file),
	archive(archive)
{
}

ZCameraReplayer::~ZCameraReplayer()
{
	// The archive must be destroyed before the file stream.
	// This is because the archive uses the file so if we destroy
	// the file first the archive will no longer have a file.
	archive.reset();
	file.reset();
}

void ZCameraReplayer::replay(LogEvent_t logEvent)
{
	// don't try handling an event that isnt a zcam event
	assert(LogEventCategory::lookupCategory(logEvent) == LogEventCategory::ZCAM);

	switch (logEvent)
	{
		case LogEvent::ZCAM_AVATAR_POSITION:
		{
			cVector3d position;
			*archive >> position;

			notify(AVATAR_POSITION, &position);
			break;
		}
	}

	return;
}

void ZCameraReplayer::startCapture()
{
	return;
}

void ZCameraReplayer::stopCapture()
{
	return;
}

void ZCameraReplayer::resetPosition()
{
	return;
}

