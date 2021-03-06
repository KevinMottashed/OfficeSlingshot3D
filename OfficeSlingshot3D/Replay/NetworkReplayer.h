#ifndef NETWORK_REPLAYER
#define NETWORK_REPLAYER

#include "stdafx.h"
#include "boost.h"

#include "LoggerProxy.h"
#include "NetworkProxy.h"
#include "ConnectionStateEnum.h"
#include "BodyPartEnum.h"
#include "PhysicsSync.h"

/**
 * The network replayer class is in charge of replaying network events.
 * The replay function is used to replay a specific log event, at which
 * point the network replayer will extract any information it needs from
 * the provided file.
 */
class NetworkReplayer : public Network
{
public:
	/**
	 * Constructor.
	 * We must pass in both the file and the archive because we can't delete the file first.
	 * @param file The file from which archive was created.
	 * @param archive The archive containing all the replay events.
	 */ 
	NetworkReplayer(boost::shared_ptr<std::ifstream> file,
					boost::shared_ptr<boost::archive::text_iarchive> archive);

	/**
	 * Destructor.
	 */
	virtual	~NetworkReplayer();

	/**
	 * Replay an event, the event must be part of the network event category.
	 * @param logEvent the event to replay.
	 */
	void replay(LogEvent_t logEvent);

	/**
	 * Start listening for connections.
	 * The replayer won't actually do anything, this is just to implement the network interface.
	 * @param userName The username used to establish the connection
	 * @return SUCCESS 
	 */ 
	virtual rc_network listen(const std::string& userName);	

	/**
	 * Attempt to connect to a peer.
	 * The replayer won't actually do anything, this is just to implement the network interface.
	 * @param ipAddress The ip address that we will connect to
	 * @param userName The username that will be used to establish the connection
	 * @return SUCCESS 
	 */
	virtual rc_network connect(const std::string& ipAddress, const std::string& userName);

	/**
	 * Disconnect from a peer.
	 * The replayer won't actually do anything, this is just to implement the network interface.
	 * @return SUCCESS 
	 */
	virtual rc_network disconnect();

	/**
	 * Send a start game message to the peer.
	 * The replayer won't actually do anything, this is just to implement the network interface.
	 * @return SUCCESS 
	 */ 
	virtual rc_network sendStartGame();
	
	/**
	 * Send a pause game message to the peer.
	 * The replayer won't actually do anything, this is just to implement the network interface.
	 * @return SUCCESS 
	 */ 
	virtual rc_network sendPauseGame();
	
	/**
	 * Send a end game message to the peer.
	 * The replayer won't actually do anything, this is just to implement the network interface.
	 * @return SUCCESS 
	 */ 
	virtual rc_network sendEndGame();

	/**
	 * Send a user name to the peer.
	 * The replayer won't actually do anything, this is just to implement the network interface.
	 * @return SUCCESS 
	 */
	virtual rc_network sendUserName(const std::string& userName);

	/**
	 * Send a chat message to the peer.
	 * The replayer won't actually do anything, this is just to implement the network interface.
	 * @return SUCCESS 
	 */
	virtual rc_network sendChatMessage(const std::string& message);
	
	/**
	 * Send a player position to the peer.
	 * The replayer won't actually do anything, this is just to implement the network interface.
	 * @return SUCCESS 
	 */
	virtual rc_network sendPlayerPosition(const cVector3d& position);

	/**
	 * Send the slingshot pullback position to the peer.
	 * The pullback position is the position of the sling when the slingshot is being fired.
	 * The replayer won't actually do anything, this is just to implement the network interface.
	 * @return SUCCESS 
	 */
	virtual rc_network sendSlingshotPullback(const cVector3d& position);

	/**
	 * Send a projectile to the peer.
	 * The replayer won't actually do anything, this is just to implement the network interface.
	 * @return SUCCESS 
	 */
	virtual rc_network sendProjectile(const Projectile& projectile);

	/**
	 * Send a slingshot release event to the peer.
	 * The replayer won't actually do anything, this is just to implement the network interface.
	 * @return SUCCESS 
	 */
	virtual rc_network sendSlingshotRelease();

	/**
	 * Send a game over message to the peer.
	 * The replayer won't actually do anything, this is just to implement the network interface.
	 * @return SUCCESS
	 */
	virtual rc_network sendGameOver();

	/**
	 * Send a message to the peer letting him know that we have been hit.	 
	 * The replayer won't actually do anything, this is just to implement the network interface.
	 * @param bodyPart The body part that was hit.
	 * @return SUCCESS
	 */
	virtual rc_network sendPlayerHit(BodyPart_t bodyPart);

	/**
	 * Send a physics sync object to the peer so that he can synchronize his game.
	 * The replayer won't actually do anything, this is just to implement the network interface.
	 * @param sync The sync object.
	 * @return SUCCESS.
	 */
	virtual rc_network sendPhysicsSync(const PhysicsSync& sync);

	/**
	 * Determines if we are connected to a peer
	 * @return true if we are connected to a peer
	 */	
	virtual bool isConnected() const;
	
	/**
	 * Determines if we are listening for connections 
	 * @return true if we are listening for connections 
	 */	
	virtual bool isListening() const;

private:
	ConnectionState_t connectionState; /**< The current state of the connection */

	boost::shared_ptr<std::ifstream> file; /**< The file from which the archive was created */
	boost::shared_ptr<boost::archive::text_iarchive> archive; /**< The archive from which the replay events will be retrieved */
};

#endif

