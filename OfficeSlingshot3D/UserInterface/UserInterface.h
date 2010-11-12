#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H

#include "stdafx.h"
#include "MainDlg.h"
#include "UserInterfaceSubject.h"
#include "UserPreferences.h"
#include "ConnectionStateEnum.h"
#include "PlayerEnum.h"
#include "GameStateEnum.h"
#include "chai3d.h"

class CMainDlg;

/**
 * An interface to this package.
 * Declares all the methods to be implemented in the
 * MFCUserInterface class.
 */
class UserInterface : public UserInterfaceSubject
{
public:
	/**
	 * Constructor.
	 */
	UserInterface();

	/**
	 * Destructor.
	 */
	virtual ~UserInterface();

	/**
	 * Changes the camera from which the vritual world will be displayed
	 * @param camera The new camera
	 */
	virtual void switchCamera(cCamera* camera)=0;

	//----------------------------------------
	// Display updates
	//----------------------------------------

	/**
	 * Display that a certain player has changed the connection state.
	 * @param state The new connection state.
	 * @param player The player that changed the connetion state.
	 */
	virtual void displayConnectionStateChanged(ConnectionState_t state, Player_t player)=0;

	/**
	 * Display that a certain player has changed the game state.
	 * @param state The new game state.
	 * @param player The player that changed the game state.
	 */
	virtual void displayGameStateChanged(GameState_t state, Player_t player)=0;

	/**
	 * Update the user interface to reflect that a connection has failed.
	 */
	virtual void displayConnectionFailed()=0;

	/**
	 * Update the user interface to reflect that the listen has failed.
	 */
	virtual void displayFailedToListen()=0;

	/**
	 * Update the user interface to reflect that a network error occurred.
	 */
	virtual void displayNetworkError()=0;

	/**
	 * Update the user interface to display a new peer message.
	 */
	virtual void displayPeerChatMessage(const std::string& message)=0;

	/**
	 * Update the user interface to display game over.
	 * @param player The player that has won the game.
	 */
	virtual void displayGameOver(Player_t player) = 0;

	/**
	 * Redraw the screen.
	 */
	virtual void paint() = 0; 

	//-----------------------------------------------
	// Notifications coming from the user interface
	//-----------------------------------------------

	/**
	 * User wants to connect to the peer.
	 */
	virtual void networkConnectButtonPushed()=0;

	/**
	 * User wants to listen for connections.
	 */
	virtual void networkListenButtonPushed()=0;

	/**
	 * User wants to disconnect to the peer.
	 */
	virtual void networkDisconnectButtonPushed()=0;

	/**
	 * User wants to send a new chat message.
	 */
	virtual void sendChatButtonPushed(const std::string& message)=0;

	/**
	 * User wants to start a new game.
	 */
	virtual void startGameButtonPushed()=0;

	/**
	 * User wants to exit the game.
	 */
	virtual void exitGameButtonPushed()=0;

	/**
	 * User wants to pause the game.
	 */
	virtual void pauseGameButtonPushed()=0;

	/**
	 * Update the user's preferences.
	 * @param preferences The new user preferences.
	 */
	virtual void changePreferences(const UserPreferences& preferences)=0;

	/**
	 * User wants to close the application.
	 */
	virtual void closeApplication()=0;
	
	/**
	 * Update the peers user name.
	 * @param name The new user name.
	 */
	virtual void setPeerUserName(const std::string& name)=0;

	/**
	 * Mutes the volume
	 * @param soundOn True if the sound is currently on and is to be muted
	 */
	virtual void muteVolume(const bool soundOn)=0;

	/**
	 * Changes the volume
	 * @param vol The new volume
	 */
	virtual void changeVolume(const int vol)=0;

	/**
	 * Sets the mute preference
	 * @param mute True if the sound needs to be muted
	 */
	virtual void setMutePref(const bool mute)=0;

	/**
	 * Sets the volume preference
	 * @param vol The new volume
	 */
	virtual void setVolPref(const int vol)=0;
};

#endif
 
