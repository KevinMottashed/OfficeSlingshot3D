#ifndef LOGGER_H
#define LOGGER_H

#include "NetworkProxy.h"
#include "FalconProxy.h"
#include "GameProxy.h"
#include "UserInterfaceProxy.h"
#include "ZCameraProxy.h"
#include "LogEvent.h"

// abstract class used for logging
class Logger :	public NetworkObserver,
				public FalconObserver,
				public GameObserver,
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
	virtual void update(GameUpdateContext context, const void* data);	
	virtual void update(ZCameraUpdateContext context, const void* data);

protected:
	// log various event of various data types
	virtual void log(LogEvent logEvent) = 0;
	virtual void log(LogEvent logEvent, rc_network error) = 0;
	virtual void log(LogEvent logEvent, const std::string& str) = 0;
	virtual void log(LogEvent logEvent, const VideoData& video) = 0;
	virtual void log(LogEvent logEvent, const cVector3d& vec) = 0;
	virtual void log(LogEvent logEvent, const Projectile& projectile) = 0;
	virtual void log(LogEvent logEvent, const UserPreferences& preferences) = 0;
};

#endif

