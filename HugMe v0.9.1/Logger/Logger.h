#ifndef LOGGER_H
#define LOGGER_H

#include "NetworkProxy.h"
#include "FalconProxy.h"
#include "GameProxy.h"
#include "UserInterfaceProxy.h"
#include "ZCameraProxy.h"
#include "LogEventEnum.h"
#include "Projectile.h"
#include "chai3d.h"

// abstract class used for logging
class Logger :	public NetworkObserver,
				public FalconObserver,
				public UserInterfaceObserver,
				public ZCameraObserver
{
public:
	Logger();
	virtual ~Logger();

	// updates from observer patterns
	virtual void update(NetworkUpdateContext context, const void* data);
	virtual void update(UserInterfaceUpdateContext context, const void* data);
	virtual void update(FalconUpdateContext context, const void* data);	
	virtual void update(ZCameraUpdateContext context, const void* data);

protected:
	// log various events of various data types
	virtual void log(LogEvent_t logEvent) = 0;
	virtual void log(LogEvent_t logEvent, unsigned int uInt) = 0;
	virtual void log(LogEvent_t logEvent, rc_network error) = 0;
	virtual void log(LogEvent_t logEvent, const std::string& str) = 0;
	virtual void log(LogEvent_t logEvent, const VideoData& video) = 0;
	virtual void log(LogEvent_t logEvent, const cVector3d& vec) = 0;
	virtual void log(LogEvent_t logEvent, const Projectile& projectile) = 0;
	virtual void log(LogEvent_t logEvent, const UserPreferences& preferences) = 0;
};

#endif

