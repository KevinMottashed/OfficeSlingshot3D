#include "UserInterfaceReplayer.h"

using namespace std;
using namespace boost;

UserInterfaceReplayer::UserInterfaceReplayer(	const UserPreferences& preferences,
												boost::shared_ptr<std::ifstream> file,
												boost::shared_ptr<boost::archive::text_iarchive> archive) :
	MFCUserInterface(preferences),
	file(file),
	archive(archive)
{
}

UserInterfaceReplayer::~UserInterfaceReplayer()
{
	// The archive must be destroyed before the file stream.
	// This is because the archive uses the file so if we destroy
	// the file first the archive will no longer have a file.
	archive.reset();
	file.reset();
}

void UserInterfaceReplayer::replay(LogEvent_t logEvent)
{
	// don't try handling an event that isnt a ui event
	assert(LogEvent::isUIEvent(logEvent));
	
	switch (logEvent)
	{
		case LogEvent::UI_CONNECT:
		{
			UserInterfaceSubject::notify(CONNECT);
			break;
		}
		case LogEvent::UI_LISTEN:
		{
			UserInterfaceSubject::notify(LISTEN);
			break;
		}
		case LogEvent::UI_DISCONNECT:
		{
			UserInterfaceSubject::notify(DISCONNECT);
			break;
		}
		case LogEvent::UI_PREFERENCES:
		{
			UserPreferences preferences;
			*archive >> preferences;

			UserInterfaceSubject::notify(PREFERENCES, &preferences);
			break;
		}
		case LogEvent::UI_START_GAME:
		{
			UserInterfaceSubject::notify(START_GAME);
			break;
		}
		case LogEvent::UI_PAUSE_GAME:
		{
			UserInterfaceSubject::notify(PAUSE_GAME);
			break;
		}
		case LogEvent::UI_EXIT_GAME:
		{
			UserInterfaceSubject::notify(EXIT_GAME);
			break;
		}
		case LogEvent::UI_CLOSE_APPLICATION:
		{
			UserInterfaceSubject::notify(EXIT_APPLICATION);

			// notify the replay watcher that the application would have been closed
			// by not closing the application we can inspect the UI, debug, etc...
			m_pMainDlg->MessageBox("APPLICATION CLOSED");
							
			break;
		}
		case LogEvent::UI_CHAT_MESSAGE:
		{
			string str;
			*archive >> str;

			// add the message to the UI
			m_pMainDlg->displayLocalChatMessage(str);

			UserInterfaceSubject::notify(CHAT_MESSAGE, &str);
			break;
		}
	}
	return;
}

