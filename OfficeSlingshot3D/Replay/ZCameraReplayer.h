#ifndef ZCAMERA_REPLAYER
#define ZCAMERA_REPLAYER

#include "stdafx.h"
#include "boost.h"

#include "LoggerProxy.h"
#include "ZCameraProxy.h"

/**
 * The zcamera replayer class is in charge of replaying zcamera events.
 * The replay function is used to replay a specific log event, at which
 * point the zcamera replayer will extract any information it needs from
 * the provided file.
 */
class ZCameraReplayer : public IZCamera
{
public:
	/**
	 * Constructor.
	 * @param file The file that the archive was created from.
	 * @param archive The archive that will be read from for replaying events.
	 */
	ZCameraReplayer(	boost::shared_ptr<std::ifstream> file,
						boost::shared_ptr<boost::archive::text_iarchive> archive);

	/**< Destructor. */
	virtual	~ZCameraReplayer();

	/**
	 * Replay an event, the event must be part of the zcamera event category.
	 * @param logEvent The event to be replayed.
	 */
	void replay(LogEvent_t logEvent);

	/**
	 * Start capturing frames from the camera.
	 * The replayer won't actually do anything this is just to implement the camera interface.
	 */
	virtual void startCapture();

	/**
	 * Stop capturing frames from the camera.
	 * The replayer won't actually do anything this is just to implement the camera interface.
	 */
	virtual void stopCapture();

private:
	boost::shared_ptr<std::ifstream> file; /**< File from which the archive reads data */
	boost::shared_ptr<boost::archive::text_iarchive> archive; /**< The archive used for retriving data */
};	

#endif
