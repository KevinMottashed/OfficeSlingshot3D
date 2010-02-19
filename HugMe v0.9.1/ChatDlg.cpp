 // ChatDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Chat.h"
#include "ChatDlg.h"
#include "ConnectDlg.h"
#include "ChatPacket.h"
#include "Sync.h"
#include "ImageDataObject.h"
#include "OptionDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChatDlg dialog

CChatDlg::CChatDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CChatDlg::IDD, pParent), m_pChatSocket(NULL), m_pTmpVideoSocket(NULL), m_bCanChat(false)
	, m_hVideoSocket(NULL), m_bIsSending(false), m_hSendThreadEvent(NULL), m_bIsReceiving(false)
	, m_pVideoForSend(NULL), m_iCurrentBuffer(0), m_dwIDVideoSend(0), m_dwIDVideoRecv(0)
	, m_strChatName(""), m_strMateName(""), m_pEmoticonDlg(NULL), m_pRichEditOle(NULL), m_pDlgVirtualPad(NULL),
	m_pDlgCellphonePad(NULL)
{
	//{{AFX_DATA_INIT(CChatDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
#ifdef USE_H263
	m_pReceivedVideo[0] = new BYTE[DEPTH_IMAGE_WIDTH * DEPTH_IMAGE_HEIGHT * 3];
	m_pReceivedVideo[1] = new BYTE[DEPTH_IMAGE_WIDTH * DEPTH_IMAGE_HEIGHT * 3];
#else
	m_pReceivedVideo[0] = new BYTE[DEPTH_IMAGE_WIDTH * DEPTH_IMAGE_HEIGHT * 4];
	m_pReceivedVideo[1] = new BYTE[DEPTH_IMAGE_WIDTH * DEPTH_IMAGE_HEIGHT * 4];
#endif
	m_pReceivedDepth[0] = new BYTE[DEPTH_IMAGE_WIDTH * DEPTH_IMAGE_HEIGHT];
	m_pReceivedDepth[1] = new BYTE[DEPTH_IMAGE_WIDTH * DEPTH_IMAGE_HEIGHT];

	m_pVideoForSend = new BYTE[DEPTH_IMAGE_WIDTH * DEPTH_IMAGE_HEIGHT * 4];
	memset(m_pVideoForSend, 0, DEPTH_IMAGE_WIDTH * DEPTH_IMAGE_HEIGHT * 4);
	m_pDepthForSend = new BYTE[DEPTH_IMAGE_WIDTH * DEPTH_IMAGE_HEIGHT];
	memset(m_pDepthForSend, 0, DEPTH_IMAGE_WIDTH * DEPTH_IMAGE_HEIGHT);
	m_pLocalVideoFlip = new BYTE[DEPTH_IMAGE_WIDTH * DEPTH_IMAGE_HEIGHT * 4];

	m_hVideoSendThread = INVALID_HANDLE_VALUE;
	m_hVideoRecvThread = INVALID_HANDLE_VALUE;
}

void CChatDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CChatDlg)
	DDX_Control(pDX, IDC_CHAT, m_richChat);
	DDX_Control(pDX, IDC_CHAT_INPUT, m_editChatInput);
	DDX_Control(pDX, IDC_BTN_EMOTICON, m_btnEmoticon);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CChatDlg, CDialog)
	//{{AFX_MSG_MAP(CChatDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_SEND, OnBtnSend)
	ON_COMMAND(ID_FILE_CONNECT, OnFileConnect)
	ON_COMMAND(ID_FILE_LISTEN, OnFileListen)
	ON_COMMAND(ID_FILE_DISCONNECT, OnFileDisconnect)
	ON_MESSAGE(WM_ON_ACCEPT, OnAccept)
	ON_MESSAGE(WM_ON_CLOSE, OnClose)
	ON_MESSAGE(WM_ON_RECEIVE, OnReceive)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_EMOTICON, OnBtnEmoticon)
	ON_COMMAND(ID_TOOLS_OPTION, OnToolsOption)
	ON_COMMAND(ID_TOOLS_VIRTUAL_PAD, OnToolsVirtualPad)
	ON_COMMAND(ID_TOOLS_CELLPHONE_PAD, OnToolsCellphonePad)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChatDlg message handlers

BOOL CChatDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// 이모티콘 버튼
	m_btnEmoticon.LoadBitmaps(IDB_EMOTICON, IDB_EMOTICON_SEL);
	m_btnEmoticon.SizeToContent();

	// Obtain the window sizes for local and remote videos
	CWnd* pLocalWnd = GetDlgItem(IDC_VIDEO_LOCAL);
	CWnd* pRemoteWnd = GetDlgItem(IDC_VIDEO_REMOTE);
	RECT localWndRect, remoteWndRect;
	pLocalWnd->GetWindowRect(&localWndRect);
	pRemoteWnd->GetWindowRect(&remoteWndRect);
	m_localWndWidth = localWndRect.right - localWndRect.left;
	m_localWndHeight = localWndRect.bottom - localWndRect.top;
	m_remoteWndWidth = remoteWndRect.right - remoteWndRect.left;
	m_remoteWndHeight = remoteWndRect.bottom - remoteWndRect.top;

	// Initializing socket
	CChatSocket::pWndMsgProc = this;	

	// Initialize HugMe system
	CWnd * pWnd = GetDlgItem(IDC_VIDEO_REMOTE);
	m_pHugMe = new HugMe(pWnd->m_hWnd);
	m_configLocal = m_pHugMe->getConfig();

	// initialize the local video view
	initVideoPreview();
#ifdef USE_H263	
	// Initialize image en/decoder
	m_imageComp = new DepthImageComp;
	m_imageComp->InitEncoder();
	m_imageComp->InitDecoder();
#endif
	m_recvPacket.isVideo = false;

	CMenu * pMenu = GetMenu();
	if( m_configLocal.m_bUseInputDevice && m_configLocal.m_typeInputDevice == VIRTUAL_TOUCHPAD ) {
		m_pDlgVirtualPad = new CVirtualPadDlg(this);
		m_pDlgVirtualPad->Create(IDD_VIRTUAL_PAD_DIALOG, this);
		pMenu->EnableMenuItem(ID_TOOLS_VIRTUAL_PAD, MF_ENABLED| MF_BYCOMMAND);
	}
	else
		pMenu->EnableMenuItem(ID_TOOLS_VIRTUAL_PAD, MF_GRAYED | MF_BYCOMMAND);

	if( m_configLocal.m_bUseInputDevice && m_configLocal.m_typeInputDevice == CELLPHONE_TOUCHPAD ) {
		m_pDlgCellphonePad = new CCellphoneDlg(this);
		m_pDlgCellphonePad->Create(IDD_CELLPHONE_DIALOG, this);

		CellphoneSocket::pWndMsgProc = (CWnd*)m_pDlgCellphonePad;

		pMenu->EnableMenuItem(ID_TOOLS_CELLPHONE_PAD, MF_ENABLED| MF_BYCOMMAND);
	} else
		pMenu->EnableMenuItem(ID_TOOLS_CELLPHONE_PAD, MF_GRAYED | MF_BYCOMMAND);

	m_richChat.SendMessage(EM_GETOLEINTERFACE, 0, (LPARAM) &m_pRichEditOle);
	ASSERT(m_pRichEditOle != NULL);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CChatDlg::OnDestroy() 
{
	CDialog::OnDestroy();

	if(hdib!=NULL) {
		::DrawDibEnd(hdib);
		::DrawDibClose(hdib);
	}

	delete m_pLocalVideoFlip;
	delete m_pReceivedVideo[0];
	delete m_pReceivedVideo[1];
	delete m_pReceivedDepth[0];
	delete m_pReceivedDepth[1];
	delete m_pVideoForSend;
	delete m_pDepthForSend;

	if(m_pDlgVirtualPad) {
		m_pDlgVirtualPad->DestroyWindow();
		delete m_pDlgVirtualPad;
	}

	if(m_pDlgCellphonePad) {
		m_pDlgCellphonePad->DestroyWindow();
		delete m_pDlgCellphonePad;
	}

	CloseSockets();
}

void CChatDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CChatDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CChatDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CChatDlg::OnBtnSend() 
{
	if (m_bCanChat == false) {
		MessageBox("먼저 접속해 주세요.");
		return;
	}

	CString strInput; // 입력한 문자열
	m_editChatInput.GetWindowText(strInput);
	if (strInput.IsEmpty()) { // 입력한 내용이 없을 경우
		return;
	}

	AddChatContent(CString(m_strChatName.c_str()) + " said:\r\n  " + strInput);
	m_editChatInput.SetWindowText("");

	// 채팅 내용 전송
	CChatPacket packet;
	packet.setType(CChatPacket::PACKET_CHAT);
	packet.writeString(strInput);
	m_pChatSocket->Send(packet);
}

void CChatDlg::OnFileConnect() 
{
	if (m_pChatSocket) {
		MessageBox("You are already connected.");
		return;
	}

	CConnectDlg dlg;
	if (dlg.DoModal() != IDOK) {
		return;
	}

	m_pChatSocket = new CChatSocket();
	if (!m_pChatSocket->Create())
	{
		MessageBox("Failed to create the chatting socket");
		CloseSockets();
		return;
	}
	if (!m_pChatSocket->Connect(dlg.m_strAddress, CHAT_PORT))
	{
		MessageBox("Failed to connect");
		CloseSockets();
		return;
	}

	m_pTmpVideoSocket = new CChatSocket();
	if (!m_pTmpVideoSocket->Create()) {
		MessageBox("Failed to create video socket.");
		CloseSockets();
		return;
	}
	
	if (!m_pTmpVideoSocket->Connect(dlg.m_strAddress, CHAT_PORT+1))
	{
		MessageBox("Failed to connect to video socket");
		CloseSockets();
		return;
	}
	m_hVideoSocket = m_pTmpVideoSocket->Detach();

	m_bCanChat = true;

	// When the connections are established safely,
	// Send the chat name	
	m_strChatName = "sunshine";
	CChatPacket packet;
	packet.setType(CChatPacket::PACKET_NAME);
	packet.writeString(CString(m_strChatName.c_str()));
	m_pChatSocket->Send(packet);

	// Send the config of this system
	CChatPacket configPacket;
	configPacket.setType(CChatPacket::PACKET_CONFIG);
	HugMeConfig config = m_pHugMe->getConfig();
	configPacket.writeByteArray((BYTE*)&config, sizeof(HugMeConfig));
	m_pChatSocket->Send(configPacket);

	// Check the devices that are described in the configuration structure
	//m_pHugMe->CheckDevices();
	// Initialize devices
	m_pHugMe->Initialize();

	// Video synchronization
	InitializeCriticalSection(&m_csVideoSend);
	InitializeCriticalSection(&m_csVideoRecv);
	
	// to start the main thread
	// tactile device, video refresh
	// 66ms --> 15Hz refresh rate
	// 33ms --> 30Hz refresh rate
	SetTimer(1, 33, NULL);
	
	//
	m_bIsSending = true;
	m_hSendThreadEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	m_bIsReceiving = true;

	// start threads for sending and receiving the data through network
	m_hVideoSendThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) VideoSendThread, (void*) this, 0, &m_dwIDVideoSend);
	m_hVideoRecvThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) VideoRecvThread, (void*) this, 0, &m_dwIDVideoRecv);

	CMenu* pMenu = GetMenu();
	pMenu->EnableMenuItem(ID_FILE_CONNECT, MF_GRAYED | MF_BYCOMMAND);
	pMenu->EnableMenuItem(ID_FILE_LISTEN, MF_GRAYED | MF_BYCOMMAND);
	pMenu->EnableMenuItem(ID_FILE_DISCONNECT, MF_ENABLED | MF_BYCOMMAND);

	//MessageBox("Connected to the server");
}

void CChatDlg::OnFileListen() 
{
	if (m_pChatSocket) {
		MessageBox("You are already connected.");
		return;
	}

	m_pChatSocket = new CChatSocket();
	if (!m_pChatSocket->Create(CHAT_PORT))
	{
		MessageBox("Failed to create the chatting socket");
		CloseSockets();
		return;
	}
	if (!m_pChatSocket->Listen())
	{
		MessageBox("Failed to listen to a connection for chat");
		CloseSockets();
		return;
	}

	m_pTmpVideoSocket = new CChatSocket();
	if (!m_pTmpVideoSocket->Create(CHAT_PORT+1)) {
		MessageBox("Failed to create video socket.");
		CloseSockets();
		return;
	}
	if (!m_pTmpVideoSocket->Listen())
	{
		MessageBox("Failed to listen to a connection for video streaming");
		CloseSockets();
		return;
	}

	CMenu* pMenu = GetMenu();
	pMenu->EnableMenuItem(ID_FILE_CONNECT, MF_GRAYED | MF_BYCOMMAND);
	pMenu->EnableMenuItem(ID_FILE_LISTEN, MF_GRAYED | MF_BYCOMMAND);
	pMenu->EnableMenuItem(ID_FILE_DISCONNECT, MF_ENABLED | MF_BYCOMMAND);
}

void CChatDlg::OnFileDisconnect() 
{
	// quit the graphic rendering
	KillTimer(1);

	// quit the threads
	m_bIsSending = false;
	SetEvent(m_hSendThreadEvent);
	m_bIsReceiving = false;

	if(m_pHugMe->isStarted()) {
		Sleep(1);
		DeleteCriticalSection(&m_csVideoSend);
		DeleteCriticalSection(&m_csVideoRecv);
	}
	
	// Uninitialize the HugMe system
	m_pHugMe->UninitRemoteDepthVideo();
	m_pHugMe->Uninitialize();

	CloseSockets();

	CMenu* pMenu = GetMenu();
	pMenu->EnableMenuItem(ID_FILE_CONNECT, MF_ENABLED| MF_BYCOMMAND);
	pMenu->EnableMenuItem(ID_FILE_LISTEN, MF_ENABLED | MF_BYCOMMAND);
	pMenu->EnableMenuItem(ID_FILE_DISCONNECT, MF_GRAYED | MF_BYCOMMAND);
}

// server의 경우에만..
LRESULT CChatDlg::OnAccept(WPARAM wParam, LPARAM lParam)
{
	CChatSocket* pSocket = (CChatSocket*) lParam;
	if (pSocket == m_pChatSocket)
	{
		m_bCanChat = true;
		// When the connections are established safely,
		// Send the chat name
		m_strChatName = "rainbow";

		CChatPacket packet;
		packet.setType(CChatPacket::PACKET_NAME);
		packet.writeString(CString(m_strChatName.c_str()));
		m_pChatSocket->Send(packet);

		// Send the config of this system
		CChatPacket configPacket;
		configPacket.setType(CChatPacket::PACKET_CONFIG);
		HugMeConfig config = m_pHugMe->getConfig();
		configPacket.writeByteArray((BYTE*)&config, sizeof(HugMeConfig));
		m_pChatSocket->Send(configPacket);
	}
	else // Video Socket
	{
		// detach the newly generated socket for video connection and use it in the other thread.
		// Since the CSocket is not thread-safe, we should do this.
		CChatSocket* pClient = (CChatSocket*) wParam;
		m_hVideoSocket = pClient->Detach();
		// delete the video socket that is listening.
		delete m_pTmpVideoSocket;
		m_pTmpVideoSocket = NULL;

		// Check the devices that are described in the configuration structure
		//m_pHugMe->CheckDevices();
		// Initialize devices
		//m_pHugMe->InitRemoteDepthVideo();
		m_pHugMe->Initialize();

		// Video synchronization
		InitializeCriticalSection(&m_csVideoSend);
		InitializeCriticalSection(&m_csVideoRecv);
	
		// to start the main thread
		// tactile device, video refresh
		// 66ms --> 15Hz refresh rate
		// 33ms --> 30Hz refresh rate
		SetTimer(1, 33, NULL);
	
		//
		m_bIsSending = true;
		m_hSendThreadEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
		m_bIsReceiving = true;

		// start threads for sending and receiving the data through network
		m_hVideoSendThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) VideoSendThread, (void*) this, 0, &m_dwIDVideoSend);
		m_hVideoRecvThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) VideoRecvThread, (void*) this, 0, &m_dwIDVideoRecv);

		//MessageBox("Connected to the client.");
	}
	return 0;
}

LRESULT CChatDlg::OnClose(WPARAM wParam, LPARAM lParam)
{
	// quit the graphic rendering
	KillTimer(1);

	// quit the threads
	m_bIsSending = false;
	SetEvent(m_hSendThreadEvent);
	m_bIsReceiving = false;

	Sleep(1);
	DeleteCriticalSection(&m_csVideoSend);
	DeleteCriticalSection(&m_csVideoRecv);	

	// Uninitialize the HugMe system
	m_pHugMe->UninitRemoteDepthVideo();
	m_pHugMe->Uninitialize();
	
	CloseSockets();

	CMenu* pMenu = GetMenu();
	pMenu->EnableMenuItem(ID_FILE_CONNECT, MF_ENABLED| MF_BYCOMMAND);
	pMenu->EnableMenuItem(ID_FILE_LISTEN, MF_ENABLED | MF_BYCOMMAND);
	pMenu->EnableMenuItem(ID_FILE_DISCONNECT, MF_GRAYED | MF_BYCOMMAND);
	return 0;
}

LRESULT CChatDlg::OnReceive(WPARAM wParam, LPARAM lParam)
{
	vector<BYTE>& queue = *(vector<BYTE>*) wParam;

	CChatPacket packet;
	while (packet.readPacket(queue))
	{
		switch (packet.getType())
		{
			case CChatPacket::PACKET_CHAT:
			{
				packet.writeChar(0); // null-terminated string을 만들기 위해 '\0'을 마지막에 추가
				CString str = (char*) packet.getPacketPtr();
				str = CString(m_strMateName.c_str()) + " said:\r\n  " + str;
				AddChatContent(str);
				break;
			}

			case CChatPacket::PACKET_EMOTICON:
			{
				AddChatContent(CString(m_strMateName.c_str()) + " sent:");
				PatternPacket* pPacket = (PatternPacket*) packet.getPacketPtr();
				BYTE* pIconData = ((BYTE*) pPacket) + pPacket->dwPacketBytes - pPacket->dwIconBytes;
				HBITMAP hBitmap = CreateBitmap(32, 32, 1, (pPacket->dwIconBytes / (32 * 32)) * 8, pIconData);
				CImageDataObject::InsertBitmap(m_pRichEditOle, hBitmap);
				AddChatContent("");
				break;
			}

			case CChatPacket::PACKET_NAME:
				packet.writeChar('\0');
				m_strMateName = (LPCSTR) packet.getPacketPtr();
				break;
			case CChatPacket::PACKET_CONFIG:
			{
				memcpy(&m_configRemote, packet.getPacketPtr(), sizeof(HugMeConfig));
				// If there is a image device in the remote site,
				// Initialize the remote video in the remote world in HugMe
				if(m_configRemote.m_bUseImageDevice && m_pHugMe->getDepthVideo() == NULL)
					m_pHugMe->InitRemoteDepthVideo();			
				if(!m_configRemote.m_bUseImageDevice && m_pHugMe->getDepthVideo())
					m_pHugMe->UninitRemoteDepthVideo();
				break;
			}
		}
	}
	return 0;
}

void CChatDlg::AddChatContent(CString strCont)
{
	CString str;
	int oldLineCnt = m_richChat.GetLineCount();
	m_richChat.SetSel(m_richChat.GetWindowTextLength(), -1);
	m_richChat.ReplaceSel(strCont + "\r\n");
	int newLineCnt = m_richChat.GetLineCount();
	m_richChat.LineScroll(newLineCnt - oldLineCnt);
}

void CChatDlg::CloseSockets()
{
	m_bCanChat = false;

	// Close sockets
	if (m_hVideoSocket)
	{
		shutdown(m_hVideoSocket, 0);
		closesocket(m_hVideoSocket);
		m_hVideoSocket = 0;
	}

	if( m_pTmpVideoSocket ) {
		delete m_pTmpVideoSocket;
		m_pTmpVideoSocket = NULL;
	}

	if( m_pChatSocket ) {
		delete m_pChatSocket;
		m_pChatSocket = NULL;
	}
}

DWORD CChatDlg::VideoSendThread(CChatDlg* pDlg)
{

	BYTE * packet;/*
	int fillColor = 0;
	DWORD lasttime = timeGetTime();
	while (1) {
		Sleep(1);
		if (!pDlg->m_pHugMe->GetConfig().m_bUseImageDevice) {
			if (timeGetTime() - lasttime > 66)
			{
				lasttime = timeGetTime();
				fillColor++;
				memset(pDlg->m_pVideoForSend, fillColor, DEPTH_IMAGE_WIDTH * DEPTH_IMAGE_HEIGHT * 4);
			}
		}*/
	while(pDlg->m_bIsSending) {
		WaitForSingleObject(pDlg->m_hSendThreadEvent, INFINITE);

		int packetSize;

		if(pDlg->m_configLocal.m_bUseImageDevice)
		{
			
			pDlg->m_sendPacket.isVideo = true;
#ifdef USE_H263
			synchronized (pDlg->m_csVideoSend)
			{
				// Compress the color video
				pDlg->m_imageComp->Compress(pDlg->m_pVideoForSend);
				pDlg->m_sendPacket.ImageSize = pDlg->m_imageComp->compSize;

				// Compress the depth video
				pDlg->m_imageComp->CompressDepth(pDlg->m_pDepthForSend);
				pDlg->m_sendPacket.DepthSize = pDlg->m_imageComp->compDepthSize;
			}
#else
			pDlg->m_sendPacket.ImageSize = DEPTH_IMAGE_WIDTH * DEPTH_IMAGE_HEIGHT * 4;
			pDlg->m_sendPacket.DepthSize = DEPTH_IMAGE_WIDTH * DEPTH_IMAGE_HEIGHT;
#endif

				//
			//pDlg->m_sendPacket.ARDataSize = 48*TOTAL_NUM_PART;
			pDlg->m_sendPacket.ARDataSize = sizeof(HumanPartPose);
		}
		else
		{
			pDlg->m_sendPacket.isVideo = false;
			pDlg->m_sendPacket.ImageSize = 0;
			pDlg->m_sendPacket.DepthSize = 0;
			pDlg->m_sendPacket.ARDataSize = 0;
		}

		if(pDlg->m_configLocal.m_bUseInputDevice) 
		{
			pDlg->m_sendPacket.isTactile = true;
			//pDlg->m_sendPacket.TactDataSize = 17;
			pDlg->m_sendPacket.TactDataSize = sizeof(ContactInfo);
		}
		else {
			pDlg->m_sendPacket.isTactile = false;
			pDlg->m_sendPacket.TactDataSize = 0;
		}

		// accumulated data size for sending
		int dataAcc = 0;
		
		// size of the packet header
		int headerSize = sizeof(PacketHeader);
		
		// allocate memory for packet
		packetSize = headerSize + pDlg->m_sendPacket.ImageSize + pDlg->m_sendPacket.DepthSize + pDlg->m_sendPacket.ARDataSize +
			pDlg->m_sendPacket.TactDataSize;			
		packet = new BYTE[packetSize];//change for actual packet size

		// append header
		memcpy(packet + dataAcc, &pDlg->m_sendPacket, headerSize);
		dataAcc += headerSize;
		
		if(pDlg->m_sendPacket.isVideo)
		{
#ifdef USE_H263
			// append color video
			memcpy(packet+dataAcc, pDlg->m_pVideoForSend, sizeof(BYTE)*pDlg->m_sendPacket.ImageSize);
			dataAcc += sizeof(BYTE)*pDlg->m_sendPacket.ImageSize;
			// append depth video
			memcpy(packet+dataAcc,pDlg->m_imageComp->compDepthImage, sizeof(BYTE)*pDlg->m_sendPacket.DepthSize);
			dataAcc += sizeof(BYTE)*pDlg->m_sendPacket.DepthSize;
#else
			synchronized (pDlg->m_csVideoSend) {
				memcpy(packet+dataAcc, pDlg->m_pVideoForSend, sizeof(BYTE)*pDlg->m_sendPacket.ImageSize);
				dataAcc += sizeof(BYTE)*pDlg->m_sendPacket.ImageSize;
				// append depth video
				memcpy(packet+dataAcc,pDlg->m_pDepthForSend, sizeof(BYTE)*pDlg->m_sendPacket.DepthSize);
				dataAcc += sizeof(BYTE)*pDlg->m_sendPacket.DepthSize;
			}
#endif

			// append human part pose data
			memcpy(packet+dataAcc, &pDlg->m_pHugMe->getLocalHumanModel().m_partPose, sizeof(HumanPartPose));
			dataAcc += sizeof(HumanPartPose);
		}
		if(pDlg->m_sendPacket.isTactile) 
		{
			// append the contact info
			memcpy(packet+dataAcc, &pDlg->m_pHugMe->getRemoteHumanModel().m_contactInfo, sizeof(ContactInfo));
			dataAcc += sizeof(ContactInfo);
		}

		//int packetSize = packet.ImageSize + 20;//change size .. *2
		/*packet = image.size|depth.size|..image..|..depth.. */
		for (int i = 0; i < packetSize;)
		{
			int ret = send(pDlg->m_hVideoSocket, (const char *)(packet + i), packetSize - i, 0);
			if (ret > 0) {
				i += ret;
			} else if (ret == 0) {
					return 0;
			} else if (ret == SOCKET_ERROR) {
				if (GetLastError() != WSAEWOULDBLOCK) {
					return 0;
				}
			}
		}
		delete [] packet;

	}
	return 0;
}

DWORD CChatDlg::VideoRecvThread(CChatDlg* pDlg) //updated according to new protocol
{
	BYTE receivedBuffer[76800];
	vector<BYTE> queue;
	while (pDlg->m_bIsReceiving) {
		Sleep(1);
		int ret = recv(pDlg->m_hVideoSocket, (char*) receivedBuffer, 76800, 0);

		switch (ret) {
		case SOCKET_ERROR:
			if (GetLastError() != WSAEWOULDBLOCK) {
				//AfxMessageBox("SOCKET_ERROR in CChatDlg::VideoRecvThread");
				return 0;
			}
			break;
		case 0:
			return 0;
			break;

		default:			
		queue.insert(queue.end(), (BYTE*) &receivedBuffer[0], (BYTE*) &receivedBuffer[ret]);

		while (1) {
			vector<BYTE>::iterator Iter;
			Iter = queue.begin();
			BYTE* pPacket = (BYTE*)(&(*Iter));
			DWORD queueSize = queue.size();
			
			// 
			int dataAcc = 0;

			int headerSize = sizeof(PacketHeader);
		
			if ((int)queueSize < headerSize) { 
				break;
			}
			memcpy(&pDlg->m_recvPacket, pPacket, headerSize);
			dataAcc += headerSize;

			if ((int)queueSize < headerSize+pDlg->m_recvPacket.ImageSize+pDlg->m_recvPacket.DepthSize+
				pDlg->m_recvPacket.ARDataSize + pDlg->m_recvPacket.TactDataSize ){
				break;
			}			
	
			if(pDlg->m_recvPacket.isVideo) {
				synchronized (pDlg->m_csVideoRecv) {
					pDlg->m_iCurrentBuffer ^= 1;
#ifdef USE_H263
					pDlg->m_imageComp->Decompress(pPacket+dataAcc,pDlg->m_recvPacket.ImageSize);
					memcpy(pDlg->m_pReceivedVideo[pDlg->m_iCurrentBuffer], pDlg->m_imageComp->rgbdata, DEPTH_IMAGE_WIDTH*DEPTH_IMAGE_HEIGHT*3*sizeof(unsigned char));
					dataAcc += pDlg->m_recvPacket.ImageSize;

					pDlg->m_imageComp->DecompressDepth(pPacket+dataAcc, pDlg->m_recvPacket.DepthSize);
					memcpy(pDlg->m_pReceivedDepth[pDlg->m_iCurrentBuffer], pDlg->m_imageComp->depthdata, DEPTH_IMAGE_WIDTH*DEPTH_IMAGE_HEIGHT*sizeof(unsigned char));
					dataAcc += pDlg->m_recvPacket.DepthSize;
#else 
					memcpy(pDlg->m_pReceivedVideo[pDlg->m_iCurrentBuffer], pPacket+dataAcc, DEPTH_IMAGE_WIDTH*DEPTH_IMAGE_HEIGHT*4*sizeof(unsigned char));
					dataAcc += pDlg->m_recvPacket.ImageSize;
					memcpy(pDlg->m_pReceivedDepth[pDlg->m_iCurrentBuffer], pPacket+dataAcc, DEPTH_IMAGE_WIDTH*DEPTH_IMAGE_HEIGHT*sizeof(unsigned char));
					dataAcc += pDlg->m_recvPacket.DepthSize;
#endif
				}

				memcpy(&pDlg->m_pHugMe->getRemoteHumanModel().m_partPose, pPacket+dataAcc, sizeof(HumanPartPose));
				dataAcc += sizeof(HumanPartPose);

				pDlg->m_pHugMe->getRemoteHumanModel().SetPartPose(pDlg->m_pHugMe->getRemoteHumanModel().m_partPose);
			}
			if(pDlg->m_recvPacket.isTactile) {
				memcpy(&pDlg->m_pHugMe->getLocalHumanModel().m_contactInfo, pPacket+dataAcc, sizeof(ContactInfo));
				dataAcc += sizeof(ContactInfo);
			}
			queue.erase(queue.begin(), queue.begin() + dataAcc);
		}
		}
	}
	return 0;
}


// changed
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
void CChatDlg::drawLocalVideo()
{
	HugMeDevice deviceLocal = m_pHugMe->getDevice();

	if(deviceLocal.m_bOnImageDevice) {
		m_pHugMe->CaptureFrame();
		synchronized (m_csVideoSend) {
			memcpy(m_pVideoForSend, m_pHugMe->getLocalVideoBuffer(), sizeof(unsigned char)*DEPTH_IMAGE_WIDTH*DEPTH_IMAGE_HEIGHT*4);
			memcpy(m_pDepthForSend, m_pHugMe->getLocalDepthBuffer(), sizeof(unsigned char)*DEPTH_IMAGE_WIDTH*DEPTH_IMAGE_HEIGHT);
		}
	}

	// when data for sending is ready, set the event to trigger sending
	SetEvent(m_hSendThreadEvent);

	
	///////////////////////////////////
	CEdit * m_wndTouchedPart;
	CEdit * m_wndTouchedPos;
	CString sTouchedPart, sTouchedPos;
	m_wndTouchedPart = (CEdit*)GetDlgItem(IDC_TOUCHED_PART);
	m_wndTouchedPos = (CEdit*)GetDlgItem(IDC_TOUCHED_POS);
	if(deviceLocal.m_bOnDisplayDevice) {
		if(m_pHugMe->getLocalHumanModel().m_contactInfo.isContacted) {
			// for debugging
			switch(m_pHugMe->getLocalHumanModel().m_contactInfo.tagConPart) {
			case CHEST:
				sTouchedPart.Format("Chest");
				if(deviceLocal.m_typeOnDisplayDevice & TAG_TACTILE_JACKET) {
					m_pHugMe->getDisplayDeviceJacket()->setActChest(m_pHugMe->getLocalHumanModel().m_contactInfo.contactPosT.x,
						m_pHugMe->getLocalHumanModel().m_contactInfo.contactPosT.y);
					m_pHugMe->getDisplayDeviceJacket()->actuate();
				}
			break;
			case RIGHT_UPPER_ARM:
				sTouchedPart.Format("Upper arm");
				if(deviceLocal.m_typeOnDisplayDevice & TAG_TACTILE_JACKET) {
					m_pHugMe->getDisplayDeviceJacket()->setActUpperArm(m_pHugMe->getLocalHumanModel().m_contactInfo.contactPosT.x,
						m_pHugMe->getLocalHumanModel().m_contactInfo.contactPosT.y);
					m_pHugMe->getDisplayDeviceJacket()->actuate();
				}
			break;
			case RIGHT_LOWER_ARM:
				sTouchedPart.Format("Forearm");
				if(deviceLocal.m_typeOnDisplayDevice & TAG_TACTILE_ARMBAND) {
					m_pHugMe->getDisplayDeviceArmband()->setActForearm(m_pHugMe->getLocalHumanModel().m_contactInfo.contactPosT.x,
						m_pHugMe->getLocalHumanModel().m_contactInfo.contactPosT.y);
					m_pHugMe->getDisplayDeviceArmband()->actuate();
				}
			break;
			}
			
			sTouchedPos.Format("%f, %f", m_pHugMe->getLocalHumanModel().m_contactInfo.contactPosT.x,
				m_pHugMe->getLocalHumanModel().m_contactInfo.contactPosT.y);
		} else {
			sTouchedPart.Format("Nothing");
			sTouchedPos.Format("Nothing");
			if(deviceLocal.m_typeOnDisplayDevice & TAG_TACTILE_JACKET) {
				m_pHugMe->getDisplayDeviceJacket()->setIntensityAll(0);
				m_pHugMe->getDisplayDeviceJacket()->actuate();
			}
			if(deviceLocal.m_typeOnDisplayDevice & TAG_TACTILE_ARMBAND) {
				m_pHugMe->getDisplayDeviceArmband()->setIntensityAll(0);
				m_pHugMe->getDisplayDeviceArmband()->actuate();
			}
		}
	} 

	m_wndTouchedPart->SetWindowText(sTouchedPart);
	m_wndTouchedPos->SetWindowText(sTouchedPos);

	if(deviceLocal.m_bOnImageDevice) {
		// Since DrawDibDraw draw the image from bottum to up,
		// the image in the memory should be flipped bottum-up.
		int stride = DEPTH_IMAGE_WIDTH;
		for(int i=0; i<DEPTH_IMAGE_HEIGHT; i++)
			memcpy(m_pLocalVideoFlip+i*DEPTH_IMAGE_WIDTH*4,
				m_pVideoForSend + DEPTH_IMAGE_WIDTH*4*(DEPTH_IMAGE_HEIGHT-i-1),
				DEPTH_IMAGE_WIDTH*4);
		
		// Draw the local video
		::DrawDibDraw(hdib,
				  m_hdc,
				  0,		// dest : left pos
				  0,		// dest : top pos
				  m_localWndWidth,					 // don't zoom x
				  m_localWndHeight,					 // don't zoom y
				  &m_bmpinfo->bmiHeader,			 // bmp header info
				  m_pLocalVideoFlip,					 // bmp data
				  0,					 // src :left
				  0,					 // src :top
				  DEPTH_IMAGE_WIDTH,				 // src : width
				  DEPTH_IMAGE_HEIGHT,				 // src : height
				  DDF_SAME_DRAW			 // use prev params....
				  );
	}
}
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////


void CChatDlg::drawRemoteVideo()
{
	if(m_recvPacket.isVideo) {
		synchronized (m_csVideoRecv) {
#ifdef USE_H263
			memcpy(m_pHugMe->getRemoteVideoBuffer(), m_pReceivedVideo[m_iCurrentBuffer], sizeof(unsigned char)*DEPTH_IMAGE_WIDTH*DEPTH_IMAGE_HEIGHT*3);
			memcpy(m_pHugMe->getRemoteDepthBuffer(), m_pReceivedDepth[m_iCurrentBuffer], sizeof(unsigned char)*DEPTH_IMAGE_WIDTH*DEPTH_IMAGE_HEIGHT);
#else
			memcpy(m_pHugMe->getRemoteVideoBuffer(), m_pReceivedVideo[m_iCurrentBuffer], sizeof(unsigned char)*DEPTH_IMAGE_WIDTH*DEPTH_IMAGE_HEIGHT*4);
			memcpy(m_pHugMe->getRemoteDepthBuffer(), m_pReceivedDepth[m_iCurrentBuffer], sizeof(unsigned char)*DEPTH_IMAGE_WIDTH*DEPTH_IMAGE_HEIGHT);
#endif
		}
	}

	if(m_recvPacket.isVideo && m_configRemote.m_bUseImageDevice && m_pHugMe->getDepthVideo()) {
		m_pHugMe->getDepthVideo()->setImageData(m_pHugMe->getRemoteVideoBuffer(), m_pHugMe->getRemoteDepthBuffer());
	}

	if(m_configLocal.m_bUseInputDevice)
	{
		switch(m_configLocal.m_typeInputDevice) {
		case FORCE_FEEDBACK_FALCON:
		case FORCE_FEEDBACK_PHANTOM:
		case ONE_FINGER_GLOVE:
			m_pHugMe->getRemoteHumanModel().proxymityCheck(m_pHugMe->getInputDevice()->getProxy()->getProxyGlobalPosition());
			if(m_pHugMe->getRemoteHumanModel().m_contactInfo.isContacted)
				{
				m_pHugMe->getContactPoint()->setPos(m_pHugMe->getRemoteHumanModel().m_contactInfo.contactPosH);
				cVector3d cntct = m_pHugMe->getRemoteHumanModel().m_contactInfo.contactPosT;
				}
			else
				m_pHugMe->getContactPoint()->setPos(CHAI_LARGE, CHAI_LARGE, CHAI_LARGE);
		break;
		case VIRTUAL_TOUCHPAD:
			if(m_pDlgVirtualPad) {
				m_pHugMe->getRemoteHumanModel().m_contactInfo.isContacted = m_pDlgVirtualPad->m_bTouched;
				m_pHugMe->getRemoteHumanModel().m_contactInfo.tagConPart = m_pDlgVirtualPad->m_selectedBodyPart;
				m_pHugMe->getRemoteHumanModel().m_contactInfo.contactPosT.x = m_pDlgVirtualPad->m_normTouchPointX;
				m_pHugMe->getRemoteHumanModel().m_contactInfo.contactPosT.y = m_pDlgVirtualPad->m_normTouchPointY;
			}
		break;
		case CELLPHONE_TOUCHPAD:
			if(m_pDlgCellphonePad) {
				m_pHugMe->getRemoteHumanModel().m_contactInfo.isContacted = m_pDlgCellphonePad->m_bTouched;
				m_pHugMe->getRemoteHumanModel().m_contactInfo.tagConPart = m_pDlgCellphonePad->m_selectedBodyPart;
				m_pHugMe->getRemoteHumanModel().m_contactInfo.contactPosT.x = m_pDlgCellphonePad->m_normTouchPointX;
				m_pHugMe->getRemoteHumanModel().m_contactInfo.contactPosT.y = m_pDlgCellphonePad->m_normTouchPointY;
			}
		break;
		}
	}
	m_pHugMe->getWorldRemote()->computeGlobalPositions(true);
	m_pHugMe->getViewport()->render();
}

// changed
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

void CChatDlg::initVideoPreview()
{
	/////////////////////////////////////////////////////
	m_bmpinfo=new BITMAPINFO;
	m_bmpinfo->bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
	m_bmpinfo->bmiHeader.biWidth=DEPTH_IMAGE_WIDTH;
	m_bmpinfo->bmiHeader.biHeight=DEPTH_IMAGE_HEIGHT;
	m_bmpinfo->bmiHeader.biPlanes=1;
	m_bmpinfo->bmiHeader.biBitCount=32;
	m_bmpinfo->bmiHeader.biCompression=0;
	m_bmpinfo->bmiHeader.biSizeImage=0;
	m_bmpinfo->bmiHeader.biXPelsPerMeter=0;
	m_bmpinfo->bmiHeader.biYPelsPerMeter=0;
	m_bmpinfo->bmiHeader.biClrUsed=0;
	m_bmpinfo->bmiHeader.biClrImportant=0;


	// Adjust display windows
	CWnd *wnd;
	CRect rect;

	// For remote video display window
	wnd=this->GetDlgItem(IDC_VIDEO_LOCAL);	// Video display window

	// Get Dialog DC
	m_hdc=wnd->GetDC()->m_hDC;
	
	// Initialize DIB for drawing...
	hdib=::DrawDibOpen();
	if(hdib!=NULL)
	{
		::DrawDibBegin(hdib,
					   m_hdc,
					   m_localWndWidth,				// don't stretch
					   m_localWndHeight,				// don't stretch
					   &m_bmpinfo->bmiHeader,
					   DEPTH_IMAGE_WIDTH,         // width of image
					   DEPTH_IMAGE_HEIGHT,        // height of image
					   0				
					   );
	
	}	
}

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

void CChatDlg::OnTimer(UINT nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	
	// For some reason, the hand tracking api does not work in the haptic thread.
	// So when we use the finger glove, this haptic thread should be here.
	HugMeDevice deviceLocal = m_pHugMe->getDevice();
	if(deviceLocal.m_bOnInputDevice && deviceLocal.m_typeOnInputDevice == ONE_FINGER_GLOVE) {
		m_pHugMe->getInputDevice()->updatePose();
		m_pHugMe->getInputDevice()->computeForces();
		cVector3d force = m_pHugMe->getInputDevice()->m_lastComputedGlobalForce;
		double force_mag = force.length();
		/*
		m_pHugMe->m_pDisplayDeviceFinger->setIntensity(0, 0, 15);
		m_pHugMe->m_pDisplayDeviceFinger->setIntensity(0, 1, 15);
		m_pHugMe->m_pDisplayDeviceFinger->setIntensity(0, 2, 15);
		m_pHugMe->m_pDisplayDeviceFinger->actuate();
		*/
		
		if(force_mag > 0.0) {
			double max_force = 7.0;
			if(force_mag > max_force)
				force_mag = max_force;
			if(m_pHugMe->getDisplayDeviceFinger()) {
				m_pHugMe->getDisplayDeviceFinger()->setActFinger(force_mag/max_force);
				m_pHugMe->getDisplayDeviceFinger()->actuate();
			}
		} else {
			if(m_pHugMe->getDisplayDeviceFinger()) {
				m_pHugMe->getDisplayDeviceFinger()->setIntensityAll(0);
				m_pHugMe->getDisplayDeviceFinger()->actuate();
			}
		}
	}

	drawLocalVideo();
	drawRemoteVideo();

	CDialog::OnTimer(nIDEvent);
}

void CChatDlg::OnBtnEmoticon() 
{
	CRect rect;
	m_btnEmoticon.GetWindowRect(&rect);

	if (m_pEmoticonDlg == NULL)
	{
		m_pEmoticonDlg = new CEmoticonDlg(this);
		m_pEmoticonDlg->Create(CEmoticonDlg::IDD);
	}
	m_pEmoticonDlg->SetWindowPos(NULL, rect.left, rect.bottom, 0, 0, SWP_NOSIZE);
	m_pEmoticonDlg->ShowWindow(SW_SHOW);

	/*
	if (!m_bCanChat) {
		return;
	}

	CChatPacket packet;
	packet.setType(CChatPacket::PACKET_EMOTICON);
	packet.writeChar(0x01);
	m_pChatSocket->Send(packet);

	AddChatContent(CString(m_strChatName.c_str()) + " smiled.");
	*/
}

void CChatDlg::SendEmoticon(Pattern* pPattern)
{
	if (!m_bCanChat)
		return;

	AddChatContent(CString(m_strChatName.c_str()) + " sent:");
	CImageDataObject::InsertBitmap(m_pRichEditOle, (HBITMAP) pPattern->pIconBitmap->m_hObject);
	AddChatContent("");

	BYTE* pBitmapData = new BYTE[32 * 32 * 4];
	DWORD dwIconBytes = pPattern->pIconBitmap->GetBitmapBits(32 * 32 * 4, pBitmapData);
	DWORD dwPointsBytes = sizeof(Pattern::Point) * pPattern->points.size();
	DWORD dwPacketBytes = sizeof(PatternPacket) + dwPointsBytes + dwIconBytes;

	PatternPacket* pPacket = (PatternPacket*) new BYTE[dwPacketBytes];
	pPacket->dwPacketBytes = dwPacketBytes;
	pPacket->dwPointsBytes = dwPointsBytes;
	pPacket->dwIconBytes = dwIconBytes;
	pPacket->samplingTime = pPattern->samplingTime;
	pPacket->nPointCount = pPattern->points.size();
	memcpy(((BYTE*) pPacket) + sizeof(PatternPacket), &pPattern->points[0], dwPointsBytes);
	memcpy(((BYTE*) pPacket) + sizeof(PatternPacket) + dwPointsBytes, pBitmapData, dwIconBytes);
	delete[] pBitmapData;
	pBitmapData = NULL;

	CChatPacket packet;
	packet.setType(CChatPacket::PACKET_EMOTICON);
	packet.writeByteArray((BYTE*) pPacket, dwPacketBytes);
	m_pChatSocket->Send(packet);
}





void CChatDlg::OnToolsOption() 
{
	// TODO: Add your command handler code here
	COptionDlg dlg(this);
	
	dlg.DoModal();

	m_pHugMe->setConfig(dlg.m_config);
	m_pHugMe->SaveConfig();

	// Check the devices
	if(m_pHugMe->isStarted() == false)
		m_pHugMe->CheckDevices();
	else
		m_pHugMe->Initialize();

	// After checking everyting, update the config.
	m_configLocal = m_pHugMe->getConfig();

	// Send the updated config to the remote user
	if(m_pHugMe->isStarted()) {
		CChatPacket configPacket;
		configPacket.setType(CChatPacket::PACKET_CONFIG);
		configPacket.writeByteArray((BYTE*)&m_configLocal, sizeof(HugMeConfig));
		m_pChatSocket->Send(configPacket);
	}

	//
	m_pHugMe->ShowContactPoint(m_configLocal.m_bShowContactpoint);
	m_pHugMe->getRemoteHumanModel().ShowAvatar(m_configLocal.m_bShowAvatar);

	// Initialize the virtual pad menu
	CMenu * pMenu = GetMenu();
	if(m_configLocal.m_bUseInputDevice && m_configLocal.m_typeInputDevice == VIRTUAL_TOUCHPAD) {
		m_pDlgVirtualPad = new CVirtualPadDlg(this);
		m_pDlgVirtualPad->Create(IDD_VIRTUAL_PAD_DIALOG, this);
		pMenu->EnableMenuItem(ID_TOOLS_VIRTUAL_PAD, MF_ENABLED| MF_BYCOMMAND);
	} else {
		if(m_pDlgVirtualPad) {
			m_pDlgVirtualPad->DestroyWindow();
			delete m_pDlgVirtualPad;
			m_pDlgVirtualPad = NULL;
		}
		pMenu->EnableMenuItem(ID_TOOLS_VIRTUAL_PAD, MF_GRAYED | MF_BYCOMMAND);
	}

	if( m_configLocal.m_bUseInputDevice && m_configLocal.m_typeInputDevice == CELLPHONE_TOUCHPAD ) {
		m_pDlgCellphonePad = new CCellphoneDlg(this);
		m_pDlgCellphonePad->Create(IDD_CELLPHONE_DIALOG, this);

		CellphoneSocket::pWndMsgProc = (CWnd*) m_pDlgCellphonePad;

		pMenu->EnableMenuItem(ID_TOOLS_CELLPHONE_PAD, MF_ENABLED| MF_BYCOMMAND);
	} else {
		if(m_pDlgCellphonePad) {
			m_pDlgCellphonePad->DestroyWindow();
			delete m_pDlgCellphonePad;
			m_pDlgCellphonePad = NULL;

			CellphoneSocket::pWndMsgProc = NULL;
		}
		pMenu->EnableMenuItem(ID_TOOLS_CELLPHONE_PAD, MF_GRAYED | MF_BYCOMMAND);
	}
}

void CChatDlg::OnToolsVirtualPad() 
{
	// TODO: Add your command handler code here
	if( m_configLocal.m_bUseInputDevice && m_configLocal.m_typeInputDevice == VIRTUAL_TOUCHPAD ) {
		if( m_pDlgVirtualPad ) {
			m_pDlgVirtualPad->ShowWindow(SW_SHOW);
			m_pDlgVirtualPad->SetFocus();
		}
		else {
			m_pDlgVirtualPad = new CVirtualPadDlg(this);
			m_pDlgVirtualPad->Create(IDD_VIRTUAL_PAD_DIALOG, this);
			m_pDlgVirtualPad->ShowWindow(SW_SHOW);
		}
	}
}

void CChatDlg::OnToolsCellphonePad() 
{
	// TODO: Add your command handler code here
	if( m_configLocal.m_bUseInputDevice && m_configLocal.m_typeInputDevice == CELLPHONE_TOUCHPAD ) {
		if( m_pDlgCellphonePad ) {
			m_pDlgCellphonePad->ShowWindow(SW_SHOW);
			m_pDlgCellphonePad->SetFocus();
		}
		else {
			m_pDlgCellphonePad = new CCellphoneDlg(this);
			m_pDlgCellphonePad->Create(IDD_CELLPHONE_DIALOG, this);
			m_pDlgCellphonePad->ShowWindow(SW_SHOW);
		}
	}	
}
