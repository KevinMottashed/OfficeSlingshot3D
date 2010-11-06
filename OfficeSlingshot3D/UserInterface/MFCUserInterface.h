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

/**
 * A class Inprementing all the major methods of the
 * UserInterface.h interface.
 * This is the main class to link the UI element classes
 * to this package's interface to the mediator.
 */

class MFCUserInterface : public UserInterface
{
public:
	/**
	 * Constructor.
	 * @param preferences The user's preferences.
	 */
	MFCUserInterface(const UserPreferences& preferences);

	/**
	 * Destructor.
	 */
	virtual ~MFCUserInterface();

	/**
	 * Changes the camera from which the vritual world will be displayed
	 * @param camera The new camera
	 */
	virtual void switchCamera(cCamera* camera);

	//----------------------------------------
	// Display updates
	//----------------------------------------

	/**
	 * Display that a certain player has changed the connection state.
	 * @param state The new connection state.
	 * @param player The player that changed the connetion state.
	 */
	virtual void displayConnectionStateChanged(ConnectionState_t state, Player_t player);

	/**
	 * Display that a certain player has changed the game state.
	 * @param state The new game state.
	 * @param player The player that changed the game state.
	 */
	virtual void displayGameStateChanged(GameState_t state, Player_t player);

	/**
	 * Update the user interface to reflect that a connection has failed.
	 */
	virtual void displayConnectionFailed();

	/**
	 * Update the user interface to reflect that the listen has failed.
	 */
	virtual void displayFailedToListen();

	/**
	 * Update the user interface to reflect that a network error occurred.
	 */
	virtual void displayNetworkError();

	/**
	 * Update the user interface to display a new peer message.
	 */
	virtual void displayPeerChatMessage(const std::string& message);

	/**
	 * Update the user interface to display a new local video frame.
	 */
	virtual void displayLocalFrame(const VideoData& video);

	/**
	 * Update the user interface to display a new peer video frame.
	 */
	virtual void displayRemoteFrame(const VideoData& video);

	/**
	 * Update the user interface to display game over.
	 * @param player The player that has won the game.
	 */
	virtual void displayGameOver(Player_t player);

	//-----------------------------------------------
	// Notifications coming from the user interface
	//-----------------------------------------------

	/**
	 * User wants to connect to the peer.
	 */
	virtual void networkConnectButtonPushed();

	/**
	 * User wants to listen for connections.
	 */
	virtual void networkListenButtonPushed();

	/**
	 * User wants to disconnect to the peer.
	 */
	virtual void networkDisconnectButtonPushed();

	/**
	 * User wants to send a new chat message.
	 */
	virtual void sendChatButtonPushed(const std::string& message);

	/**
	 * User wants to start a new game.
	 */
	virtual void startGameButtonPushed();

	/**
	 * User wants to exit the game.
	 */
	virtual void exitGameButtonPushed();

	/**
	 * User wants to pause the game.
	 */
	virtual void pauseGameButtonPushed();

	/**
	 * Update the user's preferences.
	 * @param preferences The new user preferences.
	 */
	virtual void changePreferences(const UserPreferences& preferences);

	/**
	 * User wants to close the application.
	 */
	virtual void closeApplication();
	
	/**
	 * Update the peers user name.
	 * @param name The new user name.
	 */
	virtual void setPeerUserName(const std::string& name);	

	/**
	 * Mutes the volume
	 * @param soundOn True if the sound is currently on and is to be muted
	 */
	virtual void muteVolume(const bool soundOn);

	/**
	 * Changes the volume
	 * @param vol The new volume
	 */
	virtual void changeVolume(const int vol);

	/**
	 * Sets the mute preference
	 * @param mute True if the sound needs to be muted
	 */
	virtual void setMutePref(const bool mute);

	/**
	 * Sets the volume preference
	 * @param vol The new volume
	 */
	virtual void setVolPref(const int vol);

	/**
	 * Returns the applications main window.
	 * @return the main window.
	 */
	CDialog* getMainWindow();

protected:
	/**
	 * The applications main window.
	 */
	CMainDlg* m_pMainDlg;
};

#endif