#ifndef USER_INTERFACE_MANAGER_H
#define USER_INTERFACE_MANAGER_H

#include "stdafx.h"
#include "MainDlg.h"
#include "VideoData.h"
#include "UserInterfaceSignals.h"
#include "UserInterfaceSubject.h"
#include "UserPreferences.h"

class CMainDlg;

class UserInterfaceManager : public UserInterfaceSubject
{
public:
	UserInterfaceManager(const UserPreferences& preferences);
	virtual ~UserInterfaceManager();

	//----------------------------------------
	// Display updates
	//----------------------------------------

	// update the user interface to reflect a change in the connection state
	void displayConnectionEstablished();
	void displayConnectionFailed();
	void displayListening();
	void displayFailedToListen();
	void displayPeerDisconnected();
	void displayNetworkError();

	// update the user interface to reflect a change in the game state
	void displayGameStarted();
	void displayGamePaused();
	void displayGameExited();

	// update the user interface to display the interaction with the peer
	void displayPeerChatMessage(const std::string& message);
	void displayLocalFrame(VideoData video);
	void displayRemoteFrame(VideoData video);

	//-----------------------------------------------
	// Notifications coming from the user interface
	//-----------------------------------------------

	// user wants to change the connection state
	void networkConnectButtonPushed();
	void networkListenButtonPushed();
	void networkDisconnectButtonPushed();

	// user wants to send chat message
	void sendChatButtonPushed(const std::string& message);

	// user wants to change the game state
	void startGameButtonPushed();
	void exitGameButtonPushed();
	void pauseGameButtonPushed();

	// update the users preferences
	void changePreferences(const UserPreferences& preferences);

	// user wants to close the application
	void closeApplication();
	
	// update the peers user name
	void setPeerUserName(const std::string& name);	

	// returns the applications main window
	CDialog* getMainWindow();

private:
	CMainDlg* m_pMainDlg;
};

#endif
 
