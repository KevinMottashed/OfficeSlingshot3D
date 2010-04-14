#if !defined(AFX_MAINDLG_H__C6E6906F_C091_400D_B656_29072BCDB657__INCLUDED_)
#define AFX_MAINDLG_H__C6E6906F_C091_400D_B656_29072BCDB657__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MainDlg.h : header file
//

#include <string>

#include "Resource.h"
#include "UserInterfaceManager.h"
#include "vfw.h"

using namespace std;

/////////////////////////////////////////////////////////////////////////////
// CMainDlg dialog

class UserInterfaceManager;

class CMainDlg : public CDialog
{
public:
	CMainDlg(UserInterfaceManager* pUserInterfaceManager, CWnd* pParent = NULL);   // standard constructor

private:
	UserInterfaceManager* pUserInterfaceManager;

	// User preference variables
	string m_userName;
	string m_ipAddress;

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
	afx_msg LRESULT OnNetworkEstablished(WPARAM, LPARAM);
	afx_msg LRESULT OnNetworkDisconnected(WPARAM, LPARAM);
	afx_msg LRESULT OnNetworkError(WPARAM, LPARAM);
	afx_msg void OnPreferencesEdit();
	afx_msg void OnStartGame();
	afx_msg void OnExitGame();
	afx_msg void OnPauseGame();
	afx_msg LRESULT OnGameStarted(WPARAM, LPARAM);
	afx_msg LRESULT OnGameExited(WPARAM, LPARAM);
	afx_msg LRESULT OnGamePaused(WPARAM, LPARAM);
	afx_msg void OnDestroy();
	afx_msg void OnSendChat();
	afx_msg LRESULT OnNewChatMessage(WPARAM, LPARAM);
	afx_msg void OnChangeChatInput();
	afx_msg LRESULT OnDisplayNewLocalFrame(WPARAM, LPARAM);
	afx_msg LRESULT OnDisplayNewRemoteFrame(WPARAM, LPARAM);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINDLG_H__C6E6906F_C091_400D_B656_29072BCDB657__INCLUDED_)
