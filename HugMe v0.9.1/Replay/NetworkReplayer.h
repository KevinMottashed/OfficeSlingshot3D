#ifndef NETWORK_REPLAYER
#define NETWORK_REPLAYER

#include "stdafx.h"
#include "boost.h"

#include "LoggerProxy.h"
#include "NetworkProxy.h"
#include "ConnectionStateEnum.h"

// The network replayer class is in charge of replaying network events
// The replay function is used to replay a specific log event, at which
// point the network replayer will extract any information it needs from
// the provided file.
class NetworkReplayer : public Network
{
public:
	// contructor, takes the archive and related file that will be used to extract data
	NetworkReplayer(boost::shared_ptr<std::ifstream> file,
					boost::shared_ptr<boost::archive::text_iarchive> archive);
	virtual	~NetworkReplayer();

	// replay an event, the event must be part of the network event category
	void replay(LogEvent_t logEvent);

	// modify the connection status
	virtual rc_network listen(const std::string& userName);	
	virtual rc_network connect(const std::string& ipAddress, const std::string& userName);
	virtual rc_network disconnect();

	// send a message to modify the game state
	virtual rc_network sendStartGame();
	virtual rc_network sendPauseGame();
	virtual rc_network sendEndGame();

	// send data to the other player
	virtual rc_network sendUserName(const std::string& userName);
	virtual rc_network sendChatMessage(const std::string& message);
	virtual rc_network sendVideoData(const VideoData& video);
	virtual rc_network sendPlayerPosition(const cVector3d& position);
	virtual rc_network sendSlingshotPosition(const cVector3d& position);
	virtual rc_network sendProjectile(const Projectile& projectile);
	virtual rc_network sendSlingshotPullback();
	virtual rc_network sendSlingshotRelease();

	// network state retrievers
	virtual bool isConnected() const;
	virtual bool isListening() const;

private:
	ConnectionState_t connectionState;

	// the file/archive used for retrieving the data associated with events
	boost::shared_ptr<std::ifstream> file;
	boost::shared_ptr<boost::archive::text_iarchive> archive;	
};

#endif

