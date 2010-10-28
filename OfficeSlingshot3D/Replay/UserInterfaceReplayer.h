#ifndef USER_INTERFACE_REPLAYER
#define USER_INTERFACE_REPLAYER

#include "stdafx.h"
#include "boost.h"

#include "LoggerProxy.h"
#include "UserInterfaceProxy.h"
#include "UserPreferences.h"

class UserInterfaceReplayer : public MFCUserInterface
{
public:
	// contructor, takes the archive and related file that will be used to extract data
	UserInterfaceReplayer(	const UserPreferences& preferences,
							boost::shared_ptr<std::ifstream> file,
							boost::shared_ptr<boost::archive::text_iarchive> archive);
	virtual	~UserInterfaceReplayer();

	// replay an event, the event must be part of the user interface category
	void replay(LogEvent_t logEvent);

private:
	// the file/archive used for retrieving the data associated with events
	boost::shared_ptr<std::ifstream> file;
	boost::shared_ptr<boost::archive::text_iarchive> archive;
};

#endif
