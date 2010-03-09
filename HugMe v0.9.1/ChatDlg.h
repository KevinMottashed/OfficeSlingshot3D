// ChatDlg.h : header file
//

#if !defined(AFX_CHATDLG_H__2D16716E_1CAE_4720_B9F0_64520D9807B9__INCLUDED_)
#define AFX_CHATDLG_H__2D16716E_1CAE_4720_B9F0_64520D9807B9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "chatsocket.h"
#include "Pattern.h"
// Z-Cam
#include "DepthCamera.h"

#include "HumanModel.h"
#include "CVector3d.h"
#include "CMatrix3d.h"

#include "HugMe.h"

// Video for Window
#include <vfw.h>

// Image compressor
#include "DepthImageComp.h"

// Virtual pad dialog
#include "VirtualPadDlg.h"
// Cellphone pad dialog
#include "CellphoneDlg.h"

// Controller
#include "Controller.h"

struct PacketHeader
{
	bool	isVideo;
	bool	isTactile;
	int		ImageSize;
	int		DepthSize;
	int		ARDataSize;
	int		TactDataSize;
};


#define CHAT_PORT	8869

//#define USE_H263

/////////////////////////////////////////////////////////////////////////////
// CChatDlg dialog

class CChatDlg : public CDialog
{
// Construction
public:
	
	CChatDlg(CWnd* pParent = NULL);	// standard constructor

#ifdef USE_H263
	// image compressor
	DepthImageComp * m_imageComp;
#endif

	

// Dialog Data
	//{{AFX_DATA(CChatDlg)
	enum { IDD = IDD_CHAT_DIALOG };
	CRichEditCtrl	m_richChat;
	CEdit	m_editChatInput;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChatDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

public:

	HugMe* getHugMeSystem();

	HugMeConfig getLocalConfig() const;
	HugMeConfig getRemoteConfig() const;

	void initVideoPreview();

private:
	void AddChatContent(CString strCont);
	void CloseSockets();

	static DWORD VideoSendThread(CChatDlg* pDlg);
	static DWORD VideoRecvThread(CChatDlg* pDlg);

	void drawRemoteVideo();
	void drawLocalVideo();

	// HugMe system
	HugMe * m_pHugMe;

	// Configuration for the local and the remote HugMe systems
	HugMeConfig m_configLocal;
	HugMeConfig m_configRemote;

	// Virtual pad dialog box
	CVirtualPadDlg * m_pDlgVirtualPad;

	// Cellphone pad dialog box
	CCellphoneDlg * m_pDlgCellphonePad;

	// parameters to draw the local video
	PBITMAPINFO m_bmpinfo;
	BITMAPINFO compbmp;
	HDC m_hdc;
	HDRAWDIB hdib;

	// local video window size
	int m_localWndWidth;
	int m_localWndHeight;

	// remote video (rendered by chai3d) window size
	int m_remoteWndWidth;
	int m_remoteWndHeight;

	// videos for sending
	unsigned char * m_pVideoForSend;
	unsigned char * m_pDepthForSend;

	// videos for receiving and their buffers
	unsigned char * m_pReceivedVideo[2];
	unsigned char * m_pReceivedDepth[2];
	int m_iCurrentBuffer;

	// flipped local video for rendering
	unsigned char * m_pLocalVideoFlip;

	// Buffers to transmit data
	PacketHeader m_sendPacket;
	PacketHeader m_recvPacket;
	
	// for RichEditCtrl
	LPRICHEDITOLE	m_pRichEditOle;

// Implementation
protected:

	HICON m_hIcon;
	// socket for sending chatting and control data
	CChatSocket* m_pChatSocket;
	// socket for store the video channel temporally
	CChatSocket* m_pTmpVideoSocket;
	// socket handle for video and hugme data
	SOCKET m_hVideoSocket;

	// mutexes, semaphores, etc.. Should all be mutable
	mutable CRITICAL_SECTION m_csVideoSend;
	mutable CRITICAL_SECTION m_csVideoRecv;
	
	HANDLE m_hVideoSendThread;
	HANDLE m_hVideoRecvThread;
	DWORD m_dwIDVideoSend;
	DWORD m_dwIDVideoRecv;

	HANDLE m_hSendThreadEvent;
	bool m_bIsSending;
	bool m_bIsReceiving;

	bool m_bCanChat;
	string m_strChatName;
	string m_strMateName;

	// Generated message map functions
	//{{AFX_MSG(CChatDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBtnSend();
	afx_msg void OnFileConnect();
	afx_msg void OnFileListen();
	afx_msg void OnFileDisconnect();
	afx_msg LRESULT OnAccept(WPARAM, LPARAM);
	afx_msg LRESULT OnClose(WPARAM, LPARAM);
	afx_msg LRESULT OnReceive(WPARAM, LPARAM);
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnToolsOption();
	afx_msg void OnToolsVirtualPad();
	afx_msg void OnToolsCellphonePad();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHATDLG_H__2D16716E_1CAE_4720_B9F0_64520D9807B9__INCLUDED_)
