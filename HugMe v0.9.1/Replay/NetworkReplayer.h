#ifndef NETWORK_REPLAYER
#define NETWORK_REPLAYER

#include "stdafx.h"
#include "boost.h"

#include "LoggerProxy.h"
#include "NetworkProxy.h"
#include "ConnectionStateEnum.h"

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
	 * Send video data to the peer.
	 * The replayer won't actually do anything, this is just to implement the network interface.
	 * @return SUCCESS 
	 */ 
	virtual rc_network sendVideoData(const VideoData& video);

	/**
	 * Send a player position to the peer.
	 * The replayer won't actually do anything, this is just to implement the network interface.
	 * @return SUCCESS 
	 */
	virtual rc_network sendPlayerPosition(const cVector3d& position);

	/**
	 * Send a slingshot position to the peer.
	 * The replayer won't actually do anything, this is just to implement the network interface.
	 * @return SUCCESS 
	 */
	virtual rc_network sendSlingshotPosition(const cVector3d& position);

	/**
	 * Send a projectile to the peer.
	 * The replayer won't actually do anything, this is just to implement the network interface.
	 * @return SUCCESS 
	 */
	virtual rc_network sendProjectile(const Projectile& projectile);

	/**
	 * Send a slingshot pullback event to the peer.
	 * The replayer won't actually do anything, this is just to implement the network interface.
	 * @return SUCCESS 
	 */
	virtual rc_network sendSlingshotPullback();

	/**
	 * Send a slingshot release event to the peer.
	 * The replayer won't actually do anything, this is just to implement the network interface.
	 * @return SUCCESS 
	 */
	virtual rc_network sendSlingshotRelease();

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

