#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H

#include "stdafx.h"
#include "MainDlg.h"
#include "VideoData.h"
#include "UserInterfaceSubject.h"
#include "UserPreferences.h"
#include "ConnectionStateEnum.h"
#include "PlayerEnum.h"

class CMainDlg;

class UserInterface : public UserInterfaceSubject
{
public:
	UserInterface();
	virtual ~UserInterface();

	//----------------------------------------
	// Display updates
	//----------------------------------------

	// display that a certain player has changed the connection state
	virtual void displayConnectionStateChanged(ConnectionStateEnum state, PlayerEnum player) = 0;

	// update the user interface to reflect a change in the connection state
	virtual void displayConnectionFailed() = 0;
	virtual void displayFailedToListen() = 0;
	virtual void displayNetworkError() = 0;

	// update the user interface to reflect a change in the game state
	virtual void displayGameStarted() = 0;
	virtual void displayGamePaused() = 0;
	virtual void displayGameExited() = 0;

	// update the user interface to display the interaction with the peer
	virtual void displayPeerChatMessage(const std::string& message) = 0;
	virtual void displayLocalFrame(VideoData video) = 0;
	virtual void displayRemoteFrame(VideoData video) = 0;

	//-----------------------------------------------
	// Notifications coming from the user interface
	//-----------------------------------------------

	// user wants to change the connection state
	virtual void networkConnectButtonPushed() = 0;
	virtual void networkListenButtonPushed() = 0;
	virtual void networkDisconnectButtonPushed() = 0;

	// user wants to send chat message
	virtual void sendChatButtonPushed(const std::string& message) = 0;

	// user wants to change the game state
	virtual void startGameButtonPushed() = 0;
	virtual void exitGameButtonPushed() = 0;
	virtual void pauseGameButtonPushed() = 0;

	// update the users preferences
	virtual void changePreferences(const UserPreferences& preferences) = 0;

	// user wants to close the application
	virtual void closeApplication() = 0;
	
	// update the peers user name
	virtual void setPeerUserName(const std::string& name) = 0;	
};

#endif
 
