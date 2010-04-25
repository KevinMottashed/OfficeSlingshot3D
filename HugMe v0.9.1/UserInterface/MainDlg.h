#ifndef MAIN_DLG_H
#define MAIN_DLG_H

#include "stdafx.h"
#include "Resource.h"
#include "UserInterfaceManager.h"
#include "VideoData.h"
#include "UserPreferences.h"

/////////////////////////////////////////////////////////////////////////////
// CMainDlg dialog

class UserInterfaceManager;

class CMainDlg : public CDialog
{
public:
	CMainDlg(UserInterfaceManager* pUserInterfaceManager, const UserPreferences& preferences, CWnd* pParent = NULL);   // standard constructor

	// visual feedback for connection state
	void displayConnectionEstablished();	
	void displayConnectionFailed();
	void displayListening();
	void displayFailedToListen();
	void displayPeerDisconnected();
	void displayNetworkError();

	// visual feedback for game state
	void displayGameStarted();
	void displayGamePaused();
	void displayGameExited();

	// visual feedback for peer interaction
	void displayPeerChatMessage(const std::string& message);
	void displayLocalChatMessage(const std::string& message);
	void displayLocalFrame(VideoData video);
	void displayRemoteFrame(VideoData video);

	// update the peers user name
	void setPeerUserName(const std::string& name);

private:
	UserInterfaceManager* pUserInterfaceManager;
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

	// Method to add a string in a new line in the text box
	void AddChatContent(CString strCont);
	
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
 
