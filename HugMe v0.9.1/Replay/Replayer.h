#ifndef REPLAYER_H
#define REPLAYER_H

#include "stdafx.h"
#include "boost.h"
#include "NetworkProxy.h"
#include "NetworkReplayer.h"
#include "UserInterfaceProxy.h"
#include "FalconProxy.h"
#include "ZCameraProxy.h"
#include "LoggerProxy.h"
#include "UserPreferences.h"

// The Replayer class is used to replay user inputs
// Depending on how the replayer it can replay any combination of
// network, UI, falcon and zcamera events.
// The replayer is meant as a testing class to produce predictable user inputs.
// This facilates unit testing as a we can give the application a set of inputs and
// expect the same output each time.
class Replayer : public MFCUserInterface, public Falcon, public ZCamera
{
public:
	// Replay inputs are loaded from a file and we need the user preferences
	// to properly display the UI
	Replayer(std::string fileName, const UserPreferences& preferences);
	virtual ~Replayer();

	// start/stop the replay
	void startReplay();
	void stopReplay();

	// initialize the various replayers
	void initializeNetworkReplayer();

	// remove the various replayers
	void removeNetworkReplayer();

	// get the various replayers
	boost::shared_ptr<NetworkReplayer> getNetworkReplayer();	

	//---------------------------------------------------------------------
	// Falcon
	//---------------------------------------------------------------------

	// start/stop polling the falcon
	virtual void startPolling();
	virtual void stopPolling();

	//---------------------------------------------------------------------
	// Z-Camera
	//---------------------------------------------------------------------

	// start/stop capturing frames from the camera
	virtual void startCapture();
	virtual void stopCapture();

private:

	// start replaying from the file
	void replay();

	// the time at which the replay was started
	boost::posix_time::ptime startTime;

	// the file and the serialization archive for the file
	// These pointers are shared between this class and all the other replayers
	boost::shared_ptr<std::ifstream> file;
	boost::shared_ptr<boost::archive::text_iarchive> archive;

	// the state of the network connection
	ConnectionState_t connectionState;

	// holds the replay thread
	std::auto_ptr<boost::thread> replayThread;

	// the replayers for each component, these pointers are shared between this class
	// and the mediator
	boost::shared_ptr<NetworkReplayer> networkReplayer;
};

#endif
