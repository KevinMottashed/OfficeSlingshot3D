#ifndef FALCON_REPLAYER
#define FALCON_REPLAYER

#include "stdafx.h"
#include "boost.h"

#include "LoggerProxy.h"
#include "FalconProxy.h"

// The falcon replayer class is in charge of replaying falcon events
// The replay function is used to replay a specific log event, at which
// point the falcon replayer will extract any information it needs from
// the provided file.
class FalconReplayer : public Falcon
{
public:
	// contructor, takes the archive and related file that will be used to extract data
	FalconReplayer(	boost::shared_ptr<std::ifstream> file,
					boost::shared_ptr<boost::archive::text_iarchive> archive);
	virtual	~FalconReplayer();

	// replay an event, the event must be part of the falcon event category
	void replay(LogEvent_t logEvent);

	// part of falcon interface, the replayer does not actually poll the falcon
	virtual void startPolling();	
	virtual void stopPolling();

private:
	// the file/archive used for retrieving the data associated with events
	boost::shared_ptr<std::ifstream> file;
	boost::shared_ptr<boost::archive::text_iarchive> archive;
};	

#endif

