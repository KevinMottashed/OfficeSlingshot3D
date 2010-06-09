#ifndef MFC_USER_INTERFACE_H
#define MFC_USER_INTERFACE_H

#include "stdafx.h"
#include "MainDlg.h"
#include "VideoData.h"
#include "UserInterfaceSignals.h"
#include "UserInterface.h"
#include "UserPreferences.h"
#include "ConnectionStateEnum.h"
#include "PlayerEnum.h"
#include "GameStateEnum.h"

class CMainDlg;

class MFCUserInterface : public UserInterface
{
public:
	MFCUserInterface(const UserPreferences& preferences);
	virtual ~MFCUserInterface();

	//----------------------------------------
	// Display updates
	//----------------------------------------

	// display that a certain player has changed the connection state
	virtual void displayConnectionStateChanged(ConnectionState_t state, Player_t player);

	// display that a certain player has changed the game state
	virtual void displayGameStateChanged(GameState_t state, Player_t player);

	// update the user interface to reflect a change in the connection state
	virtual void displayConnectionFailed();
	virtual void displayFailedToListen();
	virtual void displayNetworkError();

	// update the user interface to display the interaction with the peer
	virtual void displayPeerChatMessage(const std::string& message);
	virtual void displayLocalFrame(VideoData video);
	virtual void displayRemoteFrame(VideoData video);

	//-----------------------------------------------
	// Notifications coming from the user interface
	//-----------------------------------------------

	// user wants to change the connection state
	virtual void networkConnectButtonPushed();
	virtual void networkListenButtonPushed();
	virtual void networkDisconnectButtonPushed();

	// user wants to send chat message
	virtual void sendChatButtonPushed(const std::string& message);

	// user wants to change the game state
	virtual void startGameButtonPushed();
	virtual void exitGameButtonPushed();
	virtual void pauseGameButtonPushed();

	// update the users preferences
	virtual void changePreferences(const UserPreferences& preferences);

	// user wants to close the application
	virtual void closeApplication();
	
	// update the peers user name
	virtual void setPeerUserName(const std::string& name);	

	// returns the applications main window
	CDialog* getMainWindow();

protected:
	CMainDlg* m_pMainDlg;
};

#endif