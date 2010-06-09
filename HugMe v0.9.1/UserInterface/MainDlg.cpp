#include "stdafx.h"
#include "chat.h"
#include "MainDlg.h"
#include "Preferences.h"

using namespace std;

CMainDlg::CMainDlg(UserInterface* pUserInterface, const UserPreferences& preferences, CWnd* pParent /*=NULL*/)
	: pUserInterface(pUserInterface),
		m_preferences(preferences),
		CDialog(CMainDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMainDlg)
	//}}AFX_DATA_INIT
}


void CMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMainDlg)
		DDX_Control(pDX, IDC_CHAT, m_richChat);
		DDX_Control(pDX, IDC_CHAT_INPUT, m_editChatInput);
		DDX_Control(pDX, IDC_SEND_CHAT, m_sendChatButton);
		DDX_Control(pDX, IDC_VIDEO, m_videoBitmap);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMainDlg, CDialog)
	//{{AFX_MSG_MAP(CMainDlg)
	ON_COMMAND(ID_NETWORK_CONNECT, OnNetworkConnect)
	ON_COMMAND(ID_NETWORK_DISCONNECT, OnNetworkDisconnect)
	ON_COMMAND(ID_NETWORK_LISTEN, OnNetworkListen)
	ON_COMMAND(ID_PREFERENCES_EDIT, OnPreferencesEdit)
	ON_COMMAND(ID_GAME_STARTGAME, OnStartGame)
	ON_COMMAND(ID_GAME_EXITGAME, OnExitGame)
	ON_COMMAND(ID_GAME_PAUSEGAME, OnPauseGame)
	ON_COMMAND(IDC_SEND_CHAT, OnSendChat)
	ON_WM_DESTROY()
	ON_EN_CHANGE(IDC_CHAT_INPUT, OnChangeChatInput)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainDlg message handlers

// method that gets called right before the Dialog is displayed on the user's screen
BOOL CMainDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// start the video before displaying the Dialog
	initLocalVideoArea();
	initRemoteVideoArea();

	CRect rect;

	// Get size and position of the picture control
	GetDlgItem(IDC_OPENGL)->GetWindowRect(rect);

	// Convert screen coordinates to client coordinates
	ScreenToClient(rect);

	// Create OpenGL Control window
	m_oglWindow.oglCreate(rect, this);

	// Setup the OpenGL Window's timer to render
	m_oglWindow.m_unpTimer = m_oglWindow.SetTimer(1, 1, 0);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// method used to connect the user to a remote user
void CMainDlg::OnNetworkConnect() 
{
	// notify the UI that the connect button has been pushed
	pUserInterface->networkConnectButtonPushed();
	return;
}

// method used to close the connection with the remote user
void CMainDlg::OnNetworkDisconnect() 
{
	// notify the UI that the disconnect button has been pushed
	pUserInterface->networkDisconnectButtonPushed();
	return;
}

// method used to listen for incoming connection attempts
void CMainDlg::OnNetworkListen() 
{
	// let the UI know that the listen button has been pushed
	pUserInterface->networkListenButtonPushed();	
	return;
}

// method opening the CPreferences dialog and updating the user preferences
void CMainDlg::OnPreferencesEdit() 
{
	CPreferences prefs(m_preferences);	

	// show the CPrefences dialog
	if(prefs.DoModal() == IDOK) 
	{
		UserPreferences preferences = prefs.getPreferences(); 
		
		// change our preferences
		m_preferences = preferences;
		pUserInterface->changePreferences(preferences);
	}
	return;	
}

// method used to start the game with the connected remote user
void CMainDlg::OnStartGame()
{
	// start the game
	pUserInterface->startGameButtonPushed();
	return;
}

// method used to exit the game with the connected remote user
void CMainDlg::OnExitGame() 
{
	// exit the game
	pUserInterface->exitGameButtonPushed();
	return;
}

// method used to pause the game with the connected remote user
void CMainDlg::OnPauseGame()
{
	// pause the game
	pUserInterface->pauseGameButtonPushed();
	return;
}

// method used to send a new chat message to the remote user
void CMainDlg::OnSendChat() 
{
	// get the typed text from the chat edit box
	CString chatInput;
	m_editChatInput.GetWindowText(chatInput);

	// check if the text is empty
	if(!chatInput.IsEmpty()) 
	{
		// send the chat message to the remote user through the network
		pUserInterface->sendChatButtonPushed((string)chatInput);

		// empty the chat edit box
		m_editChatInput.SetWindowText("");

		ostringstream os;
		os << m_preferences.name << " : " << (string)chatInput;

		// add chat text on the local user's text area
		AddChatContent(os.str().c_str());
	}
}

// method used to enable/disable the send chat button depending on the chat edit box text
void CMainDlg::OnChangeChatInput()
{
	// get the typed text from the chat edit box
	CString newChatInput;
	m_editChatInput.GetWindowText(newChatInput);

	if(!newChatInput.IsEmpty()) {
		// Enable the send button if the text is not empty
		m_sendChatButton.EnableWindow(TRUE);
	} else {
		// Disable the send button if the text is empty
		m_sendChatButton.EnableWindow(FALSE);
	}
}

// method used to initiate a new local DIB
void CMainDlg::initLocalVideoArea()
{
	// assemble the bitmap header
	m_bmpinfo_local=new BITMAPINFO;
	m_bmpinfo_local->bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
	m_bmpinfo_local->bmiHeader.biWidth=320;
	m_bmpinfo_local->bmiHeader.biHeight=240;
	m_bmpinfo_local->bmiHeader.biPlanes=1;
	m_bmpinfo_local->bmiHeader.biBitCount=32;
	m_bmpinfo_local->bmiHeader.biCompression=0;
	m_bmpinfo_local->bmiHeader.biSizeImage=0;
	m_bmpinfo_local->bmiHeader.biXPelsPerMeter=0;
	m_bmpinfo_local->bmiHeader.biYPelsPerMeter=0;
	m_bmpinfo_local->bmiHeader.biClrUsed=0;
	m_bmpinfo_local->bmiHeader.biClrImportant=0;

	RECT localWndRect;

	//video display window
	wnd_local = this->GetDlgItem(IDC_VIDEO_LOCAL);

	wnd_local->GetWindowRect(&localWndRect);

	// set the height and width to match the video window
	m_localWndWidth_local = localWndRect.right - localWndRect.left;
	m_localWndHeight_local = localWndRect.bottom - localWndRect.top;

	//get Dialog DC
	m_hdc_local=wnd_local->GetDC()->m_hDC;
	
	//initialize DIB for drawing
	hdib_local=DrawDibOpen();
	if(hdib_local!=NULL)
	{
		::DrawDibBegin(hdib_local,
					   m_hdc_local,
					   m_localWndWidth_local,		//don't stretch
					   m_localWndHeight_local,	//don't stretch
					   &m_bmpinfo_local->bmiHeader,
					   IMAGE_WIDTH,          //width of image
					   IMAGE_HEIGHT,         //height of image
					   0				
					   );
	
	}
}

// method used to close the DIB
void CMainDlg::closeLocalVideoArea()
{
	// ends and closes the DIB if it was previously initialized
	if(hdib_local!=NULL) {
		DrawDibEnd(hdib_local);
		DrawDibClose(hdib_local);
	}
}

// method used to initiate a new local DIB
void CMainDlg::initRemoteVideoArea()
{
	// assemble the bitmap header
	m_bmpinfo_remote=new BITMAPINFO;
	m_bmpinfo_remote->bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
	m_bmpinfo_remote->bmiHeader.biWidth=320;
	m_bmpinfo_remote->bmiHeader.biHeight=240;
	m_bmpinfo_remote->bmiHeader.biPlanes=1;
	m_bmpinfo_remote->bmiHeader.biBitCount=32;
	m_bmpinfo_remote->bmiHeader.biCompression=0;
	m_bmpinfo_remote->bmiHeader.biSizeImage=0;
	m_bmpinfo_remote->bmiHeader.biXPelsPerMeter=0;
	m_bmpinfo_remote->bmiHeader.biYPelsPerMeter=0;
	m_bmpinfo_remote->bmiHeader.biClrUsed=0;
	m_bmpinfo_remote->bmiHeader.biClrImportant=0;

	RECT localWndRect;

	//video display window
	wnd_remote = this->GetDlgItem(IDC_VIDEO_REMOTE);

	wnd_remote->GetWindowRect(&localWndRect);

	// set the height and width to match the video window
	m_localWndWidth_remote = localWndRect.right - localWndRect.left;
	m_localWndHeight_remote = localWndRect.bottom - localWndRect.top;

	//get Dialog DC
	m_hdc_remote=wnd_remote->GetDC()->m_hDC;
	
	//initialize DIB for drawing
	hdib_remote=DrawDibOpen();
	if(hdib_remote!=NULL)
	{
		::DrawDibBegin(hdib_remote,
					   m_hdc_remote,
					   m_localWndWidth_remote,		//don't stretch
					   m_localWndHeight_remote,	//don't stretch
					   &m_bmpinfo_remote->bmiHeader,
					   IMAGE_WIDTH,          //width of image
					   IMAGE_HEIGHT,         //height of image
					   0				
					   );
	
	}
}

// method used to close the DIB
void CMainDlg::closeRemoteVideoArea()
{
	// ends and closes the DIB if it was previously initialized
	if(hdib_remote!=NULL) {
		DrawDibEnd(hdib_remote);
		DrawDibClose(hdib_remote);
	}
}

// util method facilitating the addition of text on the text area
void CMainDlg::AddChatContent(CString strCont)
{
	// get the text already in the text area
	CString str;
	int oldLineCnt = m_richChat.GetLineCount();

	// add the new text at the end
	m_richChat.SetSel(m_richChat.GetWindowTextLength(), -1);
	m_richChat.ReplaceSel(strCont + "\r\n");

	// scroll to the bottom of the text area
	int newLineCnt = m_richChat.GetLineCount();
	m_richChat.LineScroll(newLineCnt - oldLineCnt);
}

// method used to intercept messages before they are processed by the UI
BOOL CMainDlg::PreTranslateMessage(MSG* pMsg)
{
	// catches the message when the user presses the Escape key
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_ESCAPE)
		{
			//MessageBox("Escape");
			// TODO Perform Escape action
			return TRUE;
		}
			
	}
	// perform the default action
	return CDialog::PreTranslateMessage(pMsg);
}

// method that gets called right before the Dialog is destroyed
void CMainDlg::OnDestroy() 
{
	// close video DIB and application
	closeLocalVideoArea();
	closeRemoteVideoArea();
	pUserInterface->closeApplication();
}

void CMainDlg::displayConnectionStateChanged(ConnectionState_t state, Player_t player)
{
	// the message that the user will see in the chat box
	ostringstream message;

	// the menu options, either enabled or disabled
	long networkConnect;
	long networkListen;
	long networkDisconnect;
	long gameStart;
	long gamePause;
	long gameExit;
	
	switch (state)
	{
		case ConnectionState::DISCONNECTED:
		{
			networkConnect = MF_ENABLED;
			networkListen = MF_ENABLED;
			networkDisconnect = MF_GRAYED;

			// all game buttons are disabled
			gameStart = gamePause = gameExit = MF_GRAYED;
			
			if (player == Player::LOCAL)
			{
				message << "Disconnected";
			}
			else
			{
				message << m_peerUserName << " has disconnected";
			}
			break;
		}
		case ConnectionState::LISTENING:
		{
			// it makes no sense for a peer to tell us that he is listening
			// as we are not even connected to him
			assert(player == Player::LOCAL);
			
			networkConnect = MF_GRAYED;
			networkListen = MF_GRAYED;
			networkDisconnect = MF_ENABLED;

			// all game buttons are disabled
			gameStart = gamePause = gameExit = MF_GRAYED;

			message << "Listening";
			break;
		}
		case ConnectionState::CONNECTED:
		{
			networkConnect = MF_GRAYED;
			networkListen = MF_GRAYED;
			networkDisconnect = MF_ENABLED;

			gameStart = MF_ENABLED;
			gamePause = MF_GRAYED;
			gameExit = MF_GRAYED;

			if (player == Player::LOCAL)
			{
				message << "Connected to " << m_peerUserName;
			}
			else
			{
				message << m_peerUserName << " has connected";
			}

			break;
		}
	}

	// change the state of the menu options
	CMenu* pMenu = GetMenu();
	pMenu->EnableMenuItem(ID_NETWORK_CONNECT, networkConnect | MF_BYCOMMAND);
	pMenu->EnableMenuItem(ID_NETWORK_LISTEN, networkListen | MF_BYCOMMAND);
	pMenu->EnableMenuItem(ID_NETWORK_DISCONNECT, networkDisconnect | MF_BYCOMMAND);

	pMenu->EnableMenuItem(ID_GAME_STARTGAME, gameStart | MF_BYCOMMAND);
	pMenu->EnableMenuItem(ID_GAME_EXITGAME, gameExit | MF_BYCOMMAND);
	pMenu->EnableMenuItem(ID_GAME_PAUSEGAME, gamePause | MF_BYCOMMAND);


	// display the message in the chat box
	AddChatContent(message.str().c_str());
	
	return;
}

void CMainDlg::displayGameStateChanged(GameState_t state, Player_t player)
{
	// the message that the user will see in the chat box
	ostringstream message;

	// the game menu options, either enabled or disabled
	long gameStart;
	long gamePause;
	long gameExit;

	switch(state)
	{
		case GameState::NOT_RUNNING:
		{
			gameStart = MF_ENABLED;
			gamePause = MF_GRAYED;
			gameExit = MF_GRAYED;

			if (player == Player::LOCAL)
			{
				message << "Exited the game";
			}
			else
			{
				message << m_peerUserName << " has exited the game";
			}
			break;
		}
		case GameState::PAUSED:
		{
			gameStart = MF_ENABLED;
			gamePause = MF_GRAYED;
			gameExit = MF_ENABLED;

			if (player == Player::LOCAL)
			{
				message << "Paused the game";
			}
			else
			{
				message << m_peerUserName << " has paused the game";
			}
			break;
		}
		case GameState::RUNNING:
		{
			gameStart = MF_GRAYED;
			gamePause = MF_ENABLED;
			gameExit = MF_ENABLED;

			if (player == Player::LOCAL)
			{
				message << "Started the game";
			}
			else
			{
				message << m_peerUserName << " has started the game";
			}
			break;
		}
	}

	// change the state of the game menu options
	CMenu* pMenu = GetMenu();
	pMenu->EnableMenuItem(ID_GAME_STARTGAME, gameStart | MF_BYCOMMAND);
	pMenu->EnableMenuItem(ID_GAME_EXITGAME, gameExit | MF_BYCOMMAND);
	pMenu->EnableMenuItem(ID_GAME_PAUSEGAME, gamePause | MF_BYCOMMAND);

	AddChatContent(message.str().c_str());
	return;
}

void CMainDlg::displayConnectionFailed()
{
	AddChatContent("Failed to connect to peer");
	return;
}

void CMainDlg::displayFailedToListen()
{
	AddChatContent("Could not listen for connections");
	return;
}

void CMainDlg::displayNetworkError()
{
	// display error message on the text area
	AddChatContent("Network error");

	// enable and disable appropriate menu items
	CMenu* pMenu = GetMenu();
	pMenu->EnableMenuItem(ID_NETWORK_CONNECT, MF_ENABLED | MF_BYCOMMAND);
	pMenu->EnableMenuItem(ID_NETWORK_LISTEN, MF_ENABLED | MF_BYCOMMAND);
	pMenu->EnableMenuItem(ID_NETWORK_DISCONNECT, MF_GRAYED | MF_BYCOMMAND);
	pMenu->EnableMenuItem(ID_GAME_STARTGAME, MF_GRAYED | MF_BYCOMMAND);
	pMenu->EnableMenuItem(ID_GAME_EXITGAME, MF_GRAYED | MF_BYCOMMAND);
	pMenu->EnableMenuItem(ID_GAME_PAUSEGAME, MF_GRAYED | MF_BYCOMMAND);
	return;
}

void CMainDlg::setPeerUserName(const std::string& name)
{
	m_peerUserName = name;
	return;
}

void CMainDlg::displayPeerChatMessage(const std::string& message)
{
	// construct message
	ostringstream os;
	os << m_peerUserName << " : " << message;

	// add feedback message on the text area
	AddChatContent(os.str().c_str());

	return;
}

void CMainDlg::displayLocalChatMessage(const std::string& message)
{
	// construct message
	ostringstream os;
	os << m_preferences.name << " : " << message;

	// add feedback message on the text area
	AddChatContent(os.str().c_str());

	return;
}

void CMainDlg::displayLocalFrame(VideoData video)
{
	// get a pointer to the start of the pixel array
	BYTE* vRGB = &video.rgb->front();

	// updates the specified m_hdc dialog element using the vRGB variable
	::DrawDibDraw(hdib_local,
				  m_hdc_local,
				  0,		// dest : left pos
				  0,		// dest : top pos
				  m_localWndWidth_local,					 // don't zoom x
				  m_localWndHeight_local,					 // don't zoom y
				  &m_bmpinfo_local->bmiHeader,			 // bmp header info
				  vRGB,					 // bmp data
				  0,					 // src :left
				  0,					 // src :top
				  IMAGE_WIDTH,          // src : width
				  IMAGE_HEIGHT,			// src : height
				  DDF_SAME_DRAW			 // use prev params....
				  );
	return;
}

void CMainDlg::displayRemoteFrame(VideoData video)
{
	// get a pointer to the start of the pixel array
	BYTE* vRGB = &video.rgb->front();

	// updates the specified m_hdc dialog element using the vRGB variable
	::DrawDibDraw(hdib_remote,
				  m_hdc_remote,
				  0,		// dest : left pos
				  0,		// dest : top pos
				  m_localWndWidth_remote,					 // don't zoom x
				  m_localWndHeight_remote,					 // don't zoom y
				  &m_bmpinfo_remote->bmiHeader,			 // bmp header info
				  vRGB,					 // bmp data
				  0,					 // src :left
				  0,					 // src :top
				  IMAGE_WIDTH,          // src : width
				  IMAGE_HEIGHT,			// src : height
				  DDF_SAME_DRAW			 // use prev params....
				  );
	return;
}