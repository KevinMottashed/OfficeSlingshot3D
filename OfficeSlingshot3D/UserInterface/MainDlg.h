#ifndef MAIN_DLG_H
#define MAIN_DLG_H

#include "stdafx.h"
#include "Resource.h"
#include "UserInterface.h"
#include "VideoData.h"
#include "UserPreferences.h"
#include "MFCOpenGLControl.h"
#include "ConnectionStateEnum.h"
#include "PlayerEnum.h"
#include "GameStateEnum.h"
#include "Audio.h"

#include "boost.h"

class UserInterface;

/**
 * A class representing the main UI element of OfficeSlingshot3D.
 * The main dialog that contains all the other
 * elements of the user interface.
 */

class CMainDlg : public CDialog
{
public:
	/**
	 * Contructor.
	 * @param pUserInterface The user interface class.
	 * @param preferences The user preferences.
	 * @param pParent The parent dialog NULL in our case.
	 */
	CMainDlg(UserInterface* pUserInterface, const UserPreferences& preferences, CWnd* pParent = NULL);   // standard constructor

	/**
	 * Display that a certain player has changed the connection state.
	 * @param state The overall application's state.
	 * @param player The player that has changed the connection's state.
	 */
	void displayConnectionStateChanged(ConnectionState_t state, Player_t player);

	/**
	 * Display that a certain player has changed the game state.
	 * @param state The overall application's state.
	 * @param player The player that has changed the game's state.
	 */
	void displayGameStateChanged(GameState_t state, Player_t player);

	/**
	 * Display that the connection to the peer has failed.
	 */
	void displayConnectionFailed();

	/**
	 * Display that request to listen has failed.
	 */
	void displayFailedToListen();

	/**
	 * Display that a network error has occurred.
	 */
	void displayNetworkError();

	/**
	 * Display a new message in the peer's message box.
	 * @param message The message to display in the message box.
	 */
	void displayPeerChatMessage(const std::string& message);

	/**
	 * Display a new message in the local user's message box.
	 * @param message The message to display in the message box.
	 */
	void displayLocalChatMessage(const std::string& message);

	/**
	 * Update the user interface to display game over.
	 * @param player The player that has won the game.
	 */
	void displayGameOver(Player_t player);

	/**
	 * Update the peers user name.
	 * @param name The new name.
	 */
	void setPeerUserName(const std::string& name);

	/**
	 * Method to add a string in a new line in the text box.
	 * @param strCont The new string to add to the chat content.
	 */
	void AddChatContent(CString strCont);

	/**
	 * Set the camera whose frames will be drawn in the open gl window
	 * @param camera the new camera
	 */
	void camera(cCamera* camera);

private:
	/**
	 * The openGL window.
	 */
	boost::shared_ptr<MFCOpenGLControl> m_oglWindow;

	/**
	 * The user interface object.
	 */
	UserInterface* pUserInterface;

	/**
	 * The user preferences object.
	 */
	UserPreferences m_preferences;

	/**
	 * The peer's user name.
	 */
	std::string m_peerUserName;

	/**
	 * A flag to determine if the sound is on or off
	 */
	bool m_soundOn;
	
	/**
	 * Inherited method to catch messages before they are sent to the UI.
	 * @param pMsg the message being intercepted
	 */
	virtual BOOL CMainDlg::PreTranslateMessage(MSG* pMsg);

	/**
	 * Inherited method to perform actions before the UI is displayed.
	 */
	virtual BOOL CMainDlg::OnInitDialog();

// Dialog Data
	//{{AFX_DATA(CMainDlg)
	enum { IDD = IDD_MAIN_DIALOG };
		CRichEditCtrl m_richChat;
		CEdit m_editChatInput;
		CButton m_sendChatButton;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CMainDlg)

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Message map functions
	//{{AFX_MSG(CMainDlg)
	afx_msg void OnNetworkConnect();
	afx_msg void OnNetworkDisconnect();
	afx_msg void OnNetworkListen();
	afx_msg void OnPreferencesEdit();
	afx_msg void OnStartGame();
	afx_msg void OnExitGame();
	afx_msg void OnPauseGame();
	afx_msg void OnDestroy();
	afx_msg void OnSendChat();
	afx_msg void OnChangeChatInput();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	afx_msg void OnStnClickedVolumeCtrl();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif
 
