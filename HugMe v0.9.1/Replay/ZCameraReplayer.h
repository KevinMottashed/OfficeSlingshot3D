#ifndef ZCAMERA_REPLAYER
#define ZCAMERA_REPLAYER

#include "stdafx.h"
#include "boost.h"

#include "LoggerProxy.h"
#include "ZCameraProxy.h"

// The zcamera replayer class is in charge of replaying zcamera events
// The replay function is used to replay a specific log event, at which
// point the zcamera replayer will extract any information it needs from
// the provided file.
class ZCameraReplayer : public IZCamera
{
public:
	// contructor, takes the archive and related file that will be used to extract data
	ZCameraReplayer(	boost::shared_ptr<std::ifstream> file,
						boost::shared_ptr<boost::archive::text_iarchive> archive);
	virtual	~ZCameraReplayer();

	// replay an event, the event must be part of the zcamera event category
	void replay(LogEvent_t logEvent);

	// start/stop capturing frames from the camera, part of the zcamera interface
	// the replayer doesn't actually capture frames from a camera
	virtual void startCapture();
	virtual void stopCapture();	

private:
	// the file/archive used for retrieving the data associated with events
	boost::shared_ptr<std::ifstream> file;
	boost::shared_ptr<boost::archive::text_iarchive> archive;
};	

#endif
