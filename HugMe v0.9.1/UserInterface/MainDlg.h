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

/////////////////////////////////////////////////////////////////////////////
// CMainDlg dialog

class UserInterface;

class CMainDlg : public CDialog
{
public:
	CMainDlg(UserInterface* pUserInterface, const UserPreferences& preferences, CWnd* pParent = NULL);   // standard constructor

	// display that a certain player has changed the connection state
	void displayConnectionStateChanged(ConnectionState_t state, Player_t player);

	// display that a certain player has changed the game state
	void displayGameStateChanged(GameState_t state, Player_t player);

	// visual feedback for connection state	
	void displayConnectionFailed();
	void displayFailedToListen();
	void displayNetworkError();

	// visual feedback for peer interaction
	void displayPeerChatMessage(const std::string& message);
	void displayLocalChatMessage(const std::string& message);
	void displayLocalFrame(const VideoData& video);
	void displayRemoteFrame(const VideoData& video);

	// update the peers user name
	void setPeerUserName(const std::string& name);

	// Method to add a string in a new line in the text box
	void AddChatContent(CString strCont);

private:

	MFCOpenGLControl m_oglWindow;

	UserInterface* pUserInterface;
	UserPreferences m_preferences;

	// peers user name
	std::string m_peerUserName;

	// Local video processing variables
	CWnd *wnd_local;

	int m_localWndWidth_local;
	int m_localWndHeight_local;

	PBITMAPINFO m_bmpinfo_local;

	HDC m_hdc_local;
	HDRAWDIB hdib_local;

	// Remote video processing variables
	CWnd *wnd_remote;

	int m_localWndWidth_remote;
	int m_localWndHeight_remote;

	PBITMAPINFO m_bmpinfo_remote;

	HDC m_hdc_remote;
	HDRAWDIB hdib_remote;



	// Methods to initialize and close video sessions
	void initLocalVideoArea();
	void closeLocalVideoArea();

	void initRemoteVideoArea();
	void closeRemoteVideoArea();
	
	// Inherited method to catch messages before they are sent to the UI
	virtual BOOL CMainDlg::PreTranslateMessage(MSG* pMsg);

	// Inherited method to perform actions before the UI is displayed
	virtual BOOL CMainDlg::OnInitDialog();

// Dialog Data
	//{{AFX_DATA(CMainDlg)
	enum { IDD = IDD_MAIN_DIALOG };
		CRichEditCtrl m_richChat;
		CEdit m_editChatInput;
		CButton m_sendChatButton;
		CStatic m_videoBitmap;
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
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif
 
