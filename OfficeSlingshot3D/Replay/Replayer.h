#ifndef REPLAYER_H
#define REPLAYER_H

#include "stdafx.h"
#include "boost.h"

#include "NetworkReplayer.h"
#include "UserInterfaceReplayer.h"
#include "FalconReplayer.h"
#include "ZCameraReplayer.h"
#include "LoggerProxy.h"
#include "UserPreferences.h"

// The Replayer class is used to replay user inputs
// Depending on how the replayer it can replay any combination of
// network, UI, falcon and zcamera events.
// The replayer is meant as a testing class to produce predictable user inputs.
// This facilates unit testing as a we can give the application a set of inputs and
// expect the same output each time.
class Replayer
{
public:
	// Replay inputs are loaded from a file
	Replayer(const std::string& fileName, const UserPreferences& preferences);
	virtual ~Replayer();

	// start/stop the replay
	void startReplay();
	void stopReplay();

	// get the various replayers
	boost::shared_ptr<NetworkReplayer> getNetworkReplayer();
	boost::shared_ptr<UserInterfaceReplayer> getUserInterfaceReplayer();
	boost::shared_ptr<FalconReplayer> getFalconReplayer();	
	boost::shared_ptr<ZCameraReplayer> getZCameraReplayer();	

private:

	// start replaying from the file
	void replay();

	// The user preferences that will be used during the replay
	UserPreferences preferences;

	// the time at which the replay was started
	boost::posix_time::ptime startTime;

	// the file and the serialization archive for the file
	// These pointers are shared between this class and all the other replayers
	boost::shared_ptr<std::ifstream> file;
	boost::shared_ptr<boost::archive::text_iarchive> archive;

	// holds the replay thread
	std::auto_ptr<boost::thread> replayThread;

	// the replayers for each component, these pointers are shared between this class
	// and the mediator
	boost::shared_ptr<NetworkReplayer> networkReplayer;
	boost::shared_ptr<UserInterfaceReplayer> uiReplayer;
	boost::shared_ptr<FalconReplayer> falconReplayer;	
	boost::shared_ptr<ZCameraReplayer> zCameraReplayer;	
};

#endif
