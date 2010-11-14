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
#include "PhysicsSync.h"

/**
 * @ingroup Logger
 * @b public
 * Abstract class used for logging.
 * This defines the interface for all loggers.
 */
class Logger :	public NetworkObserver,
				public FalconObserver,
				public UserInterfaceObserver,
				public ZCameraObserver
{
public:	
	Logger(); /**< Constructor. */	
	virtual ~Logger(); /**< Destructor. */

	/**
	 * Handles an update coming from the network.
	 * @param[in] context The context of the update.
	 * @param[in] data The data associated with the update.
	 */
	virtual void update(NetworkUpdateContext context, const void* data);

	/**
	 * Handles an update coming from the user interface.
	 * @param[in] context The context of the update.
	 * @param[in] data The data associated with the update.
	 */	
	virtual void update(UserInterfaceUpdateContext context, const void* data);

	/**
	 * Handles an update coming from the falcon.
	 * @param[in] context The context of the update.
	 * @param[in] data The data associated with the update.
	 */
	virtual void update(FalconUpdateContext context, const void* data);

	/**
	 * Handles an update coming from the zcamera.
	 * @param[in] context The context of the update.
	 * @param[in] data The data associated with the update.
	 */
	virtual void update(ZCameraUpdateContext context, const void* data);

protected:
	/**
	 * Log an event that has no associated data.
	 * @param[in] logEvent The event to be logged.
	 */
	virtual void log(LogEvent_t logEvent) = 0;
	
	/**
	 * Log an event with data of type unsigned int.
	 * @param[in] logEvent The event to be logged.
	 * @param[in] uInt The associated data.
	 */
	virtual void log(LogEvent_t logEvent, unsigned int uInt) = 0;
	
	/**
	 * Log an event with data of type rc_network.
	 * @param[in] logEvent The event to be logged.
	 * @param[in] error The associated data.
	 */
	virtual void log(LogEvent_t logEvent, rc_network error) = 0;
	
	/**
	 * Log an event with data of type string.
	 * @param[in] logEvent The event to be logged.
	 * @param[in] str The associated data.
	 */
	virtual void log(LogEvent_t logEvent, const std::string& str) = 0;
	
	/**
	 * Log an event with data of type cVector3d.
	 * @param[in] logEvent The event to be logged.
	 * @param[in] vec The associated data.
	 */
	virtual void log(LogEvent_t logEvent, const cVector3d& vec) = 0;
	
	/**
	 * Log an event with data of type Projectile.
	 * @param[in] logEvent The event to be logged.
	 * @param[in] projectile The associated data.
	 */
	virtual void log(LogEvent_t logEvent, const Projectile& projectile) = 0;
	
	/**
	 * Log an event with data of type UserPreferences.
	 * @param[in] logEvent The event to be logged.
	 * @param[in] preferences The associated data.
	 */
	virtual void log(LogEvent_t logEvent, const UserPreferences& preferences) = 0;
	
	/**
	 * Log an event with data of type PhysicsSync.
	 * @param[in] logEvent The event to be logged.
	 * @param[in] sync The associated data.
	 */
	virtual void log(LogEvent_t logEvent, const PhysicsSync& sync) = 0;
};

#endif

