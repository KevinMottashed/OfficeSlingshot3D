// MainDlg.cpp : implementation file
//

#include <iostream>
#include <fstream>
#include <sstream>

#include <string>

#include "stdafx.h"
#include "chat.h"
#include "MainDlg.h"
#include "Preferences.h"
#include "vfw.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace std;

CMainDlg::CMainDlg(UserInterfaceManager* m_pUserInterfaceManager, CWnd* pParent /*=NULL*/)
	: pUserInterfaceManager(pUserInterfaceManager), CDialog(CMainDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMainDlg)
		// get the userName and ipAddress from the file if already created
		string line;
		ifstream myfile ("userPreferences.txt");
		if (myfile.is_open()) {
			getline (myfile, line);
			m_userName = line;
			getline (myfile, line);
			m_ipAddress = line;
			myfile.close();
		} else {
			// set the userName and ipAddress to the default values
			m_userName = "UserName";
			m_ipAddress = "127.0.0.1";
		}
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
	ON_COMMAND(IDC_SEND_CHAT, OnSendChat)
	ON_MESSAGE(WM_ON_CONNECT, OnNetworkEstablished)
	ON_MESSAGE(WM_ON_DISCONNECT,OnNetworkDisconnected)
	ON_MESSAGE(WM_ON_NETWORK_ERROR,OnNetworkError)
	ON_MESSAGE(WM_ON_START_GAME,OnGameStarted)
	ON_MESSAGE(WM_ON_EXIT_GAME,OnGameExited)
	ON_MESSAGE(WM_ON_NEW_CHAT_MESSAGE,OnNewChatMessage)
	ON_MESSAGE(WM_ON_NEW_FRAME, OnDisplayNewFrame)
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
	initVideoArea();

	return TRUE;
}

// method used to connect the user to a remote user
void CMainDlg::OnNetworkConnect() 
{
	// call the connect method of the networkManager through the controller
	rc_network status = pUserInterfaceManager->networkConnectButtonPushed(m_ipAddress.c_str(), m_userName);

	if (status == SUCCESS){

		// enable and disable appropriate menu items
		CMenu* pMenu = GetMenu();
		pMenu->EnableMenuItem(ID_NETWORK_CONNECT, MF_GRAYED | MF_BYCOMMAND);
		pMenu->EnableMenuItem(ID_NETWORK_LISTEN, MF_GRAYED | MF_BYCOMMAND);
		pMenu->EnableMenuItem(ID_NETWORK_DISCONNECT, MF_ENABLED | MF_BYCOMMAND);

		pMenu->EnableMenuItem(ID_GAME_STARTGAME, MF_ENABLED | MF_BYCOMMAND);
	} else {
		// show error message on the text area
		AddChatContent(lookup(status).c_str());
	}
	
}

// method used to close the connection with the remote user
void CMainDlg::OnNetworkDisconnect() 
{
	// call the disconnect method of the networkManager through the controller
	rc_network status = pUserInterfaceManager->networkDisconnectButtonPushed();

	if (status == SUCCESS){

		// enable and disable appropriate menu items
		CMenu* pMenu = GetMenu();
		pMenu->EnableMenuItem(ID_NETWORK_CONNECT, MF_ENABLED | MF_BYCOMMAND);
		pMenu->EnableMenuItem(ID_NETWORK_LISTEN, MF_ENABLED | MF_BYCOMMAND);
		pMenu->EnableMenuItem(ID_NETWORK_DISCONNECT, MF_GRAYED | MF_BYCOMMAND);

		pMenu->EnableMenuItem(ID_GAME_STARTGAME, MF_GRAYED | MF_BYCOMMAND);
		pMenu->EnableMenuItem(ID_GAME_EXITGAME, MF_GRAYED | MF_BYCOMMAND);

		// give feedback to the user on the text area
		AddChatContent("Disconnected");
	} else {
		// show error on the text area
		AddChatContent(lookup(status).c_str());
	}
	
}

// method used to listen for incoming connection attempts
void CMainDlg::OnNetworkListen() 
{
	// call the listen method of the networkManager through the controller
	rc_network status = pUserInterfaceManager->networkListenButtonPushed(m_userName);

	if (status == SUCCESS){

		// enable and disable appropriate menu items
		CMenu* pMenu = GetMenu();
		pMenu->EnableMenuItem(ID_NETWORK_CONNECT, MF_GRAYED | MF_BYCOMMAND);
		pMenu->EnableMenuItem(ID_NETWORK_LISTEN, MF_GRAYED | MF_BYCOMMAND);
		pMenu->EnableMenuItem(ID_NETWORK_DISCONNECT, MF_ENABLED | MF_BYCOMMAND);

		// give user feedback on the text area
		AddChatContent("Listening");
	} else {
		// show error on the text area
		AddChatContent(lookup(status).c_str());
	}
	
}

// method notifying the user that a remote user has connected to him
LRESULT CMainDlg::OnNetworkEstablished(WPARAM wParam, LPARAM lParam)
{
	// construct message
	string* remoteUserName = (string*) wParam;
	ostringstream os;
	os << *remoteUserName << " has joined the game";

	// add feedback message on the text area
	AddChatContent(os.str().c_str());

	// enable the startGame menu item
	CMenu* pMenu = GetMenu();
	pMenu->EnableMenuItem(ID_GAME_STARTGAME, MF_ENABLED | MF_BYCOMMAND);

	return 0;
}

// method notifying the user that a remote user has closed the connection to him
LRESULT CMainDlg::OnNetworkDisconnected(WPARAM wParam, LPARAM lParam)
{
	// construct message
	string* remoteUserName = (string*) wParam;
	ostringstream os;
	os << *remoteUserName << " has disconnected from the game";

	// add feedback message on the text area
	AddChatContent(os.str().c_str());

	// enable and disable appropriate menu items
	CMenu* pMenu = GetMenu();
	pMenu->EnableMenuItem(ID_NETWORK_CONNECT, MF_ENABLED | MF_BYCOMMAND);
	pMenu->EnableMenuItem(ID_NETWORK_LISTEN, MF_ENABLED | MF_BYCOMMAND);
	pMenu->EnableMenuItem(ID_NETWORK_DISCONNECT, MF_GRAYED | MF_BYCOMMAND);

	pMenu->EnableMenuItem(ID_GAME_STARTGAME, MF_GRAYED | MF_BYCOMMAND);
	pMenu->EnableMenuItem(ID_GAME_EXITGAME, MF_GRAYED | MF_BYCOMMAND);

	return 0;
}

// method notifying the user that a network error has occured
LRESULT CMainDlg::OnNetworkError(WPARAM wParam, LPARAM lParam)
{
	rc_network* error = (rc_network*) wParam;

	// display error message on the text area
	AddChatContent(lookup(*error).c_str());

	// enable and disable appropriate menu items
	CMenu* pMenu = GetMenu();
	pMenu->EnableMenuItem(ID_NETWORK_CONNECT, MF_ENABLED | MF_BYCOMMAND);
	pMenu->EnableMenuItem(ID_NETWORK_LISTEN, MF_ENABLED | MF_BYCOMMAND);
	pMenu->EnableMenuItem(ID_NETWORK_DISCONNECT, MF_GRAYED | MF_BYCOMMAND);

	pMenu->EnableMenuItem(ID_GAME_STARTGAME, MF_GRAYED | MF_BYCOMMAND);
	pMenu->EnableMenuItem(ID_GAME_EXITGAME, MF_GRAYED | MF_BYCOMMAND);

	return 0;
}

// method opening the CPreferences dialog and updating the user preferences
void CMainDlg::OnPreferencesEdit() 
{
	CPreferences prefs;

	// show the CPrefences dialog
	if(prefs.DoModal() == IDOK) {

		// get the dialog attributes
		m_userName = prefs.m_userName;
		m_ipAddress = prefs.m_strAddress;

		int armBandPort = prefs.m_armBandPort;
		int jacketPort = prefs.m_jacketPort;
		
		// set armBand and jacket ports through the controller
		pUserInterfaceManager->changeArmBandPort(armBandPort);
		pUserInterfaceManager->changeJacketPort(jacketPort);

		// write new userName and ipAddress to the file
		ofstream myfile;
		myfile.open ("userPreferences.txt");
		myfile << (string)prefs.m_userName << "\n";
		myfile << (string)prefs.m_strAddress;
		myfile.close();
	}
	
}

// method used to start the game with the connected remote user
void CMainDlg::OnStartGame()
{
	// start the game
	pUserInterfaceManager->startGameButtonPushed();

	// enable and disable appropriate menu items
	CMenu* pMenu = GetMenu();
	pMenu->EnableMenuItem(ID_NETWORK_CONNECT, MF_GRAYED | MF_BYCOMMAND);
	pMenu->EnableMenuItem(ID_NETWORK_LISTEN, MF_GRAYED | MF_BYCOMMAND);
	pMenu->EnableMenuItem(ID_NETWORK_DISCONNECT, MF_ENABLED | MF_BYCOMMAND);

	pMenu->EnableMenuItem(ID_GAME_STARTGAME, MF_GRAYED | MF_BYCOMMAND);
	pMenu->EnableMenuItem(ID_GAME_EXITGAME, MF_ENABLED | MF_BYCOMMAND);

	// give user feedback on the text area
	AddChatContent("Started the game");
}

// method used to exit the game with the connected remote user
void CMainDlg::OnExitGame() 
{
	// exit the game
	pUserInterfaceManager->exitGameButtonPushed();

	// enable and disable appropriate menu items
	CMenu* pMenu = GetMenu();
	pMenu->EnableMenuItem(ID_NETWORK_CONNECT, MF_GRAYED | MF_BYCOMMAND);
	pMenu->EnableMenuItem(ID_NETWORK_LISTEN, MF_GRAYED | MF_BYCOMMAND);
	pMenu->EnableMenuItem(ID_NETWORK_DISCONNECT, MF_ENABLED | MF_BYCOMMAND);

	pMenu->EnableMenuItem(ID_GAME_STARTGAME, MF_ENABLED | MF_BYCOMMAND);
	pMenu->EnableMenuItem(ID_GAME_EXITGAME, MF_GRAYED | MF_BYCOMMAND);

	// give user feedback on the text area
	AddChatContent("Exited the game");
}

// method notifying the user that the remote user has started the game
LRESULT CMainDlg::OnGameStarted(WPARAM wParam, LPARAM lParam)
{
	// construct message
	string* remoteUserName = (string*) wParam;
	ostringstream os;
	os << *remoteUserName << " has started the game";

	// add feedback message on the text area
	AddChatContent(os.str().c_str());

	// enable and disable appropriate menu items
	CMenu* pMenu = GetMenu();
	pMenu->EnableMenuItem(ID_GAME_STARTGAME, MF_GRAYED | MF_BYCOMMAND);
	pMenu->EnableMenuItem(ID_GAME_EXITGAME, MF_ENABLED | MF_BYCOMMAND);

	return 0;
}

// method notifying the user that the remote user has exited the game
LRESULT CMainDlg::OnGameExited(WPARAM wParam, LPARAM lParam)
{
	// construc message
	string* remoteUserName = (string*) wParam;
	ostringstream os;
	os << *remoteUserName << " has exited the game";

	// add feedback message on the text area
	AddChatContent(os.str().c_str());

	// enable and disable appropriate menu items
	CMenu* pMenu = GetMenu();
	pMenu->EnableMenuItem(ID_GAME_STARTGAME, MF_ENABLED | MF_BYCOMMAND);
	pMenu->EnableMenuItem(ID_GAME_EXITGAME, MF_GRAYED | MF_BYCOMMAND);

	return 0;
}

// method used to send a new chat message to the remote user
void CMainDlg::OnSendChat() 
{
	// get the typed text from the chat edit box
	CString chatInput;
	m_editChatInput.GetWindowText(chatInput);

	// check if the text is empty
	if(!chatInput.IsEmpty()) {

		// send the chat message to the remote user through the network
		rc_network status = pUserInterfaceManager->sendChatButtonPushed((string)chatInput);

		if (status == SUCCESS){

			// empty the chat edit box
			m_editChatInput.SetWindowText("");

			ostringstream os;
			os << m_userName << " : " << (string)chatInput;

			// add chat text on the local user's text area
			AddChatContent(os.str().c_str());
		} else {
			// show error on the text area
			AddChatContent(lookup(status).c_str());
		}
	}
}

// method notifying the user that the remote user has sent him a new chat message
LRESULT CMainDlg::OnNewChatMessage(WPARAM wParam, LPARAM lParam) 
{
	// construct message
	string* remoteUserName = (string*) wParam;
	string* message = (string*) lParam;
	ostringstream os;
	os << *remoteUserName << " : " << *message;

	// add feedback message on the text area
	AddChatContent(os.str().c_str());

	// empty the chat edit box
	m_editChatInput.SetWindowText("");

	return 0;
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

// method used to display a new frame on the UI
LRESULT CMainDlg::OnDisplayNewFrame(WPARAM wParam, LPARAM lParam)
{
	unsigned char* vRGB = (unsigned char*) wParam;

	// updates the specified m_hdc dialog element using the vRGB variable
	::DrawDibDraw(hdib,
				  m_hdc,
				  0,		// dest : left pos
				  0,		// dest : top pos
				  m_localWndWidth,					 // don't zoom x
				  m_localWndHeight,					 // don't zoom y
				  &m_bmpinfo->bmiHeader,			 // bmp header info
				  vRGB,					 // bmp data
				  0,					 // src :left
				  0,					 // src :top
				  ZCameraManager::IMAGE_WIDTH,          // src : width
				  ZCameraManager::IMAGE_HEIGHT,			// src : height
				  DDF_SAME_DRAW			 // use prev params....
				  );
	return 0;
}

// method used to initiate a new DIB
void CMainDlg::initVideoArea()
{
	// assemble the bitmap header
	m_bmpinfo=new BITMAPINFO;
	m_bmpinfo->bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
	m_bmpinfo->bmiHeader.biWidth=320;
	m_bmpinfo->bmiHeader.biHeight=240;
	m_bmpinfo->bmiHeader.biPlanes=1;
	m_bmpinfo->bmiHeader.biBitCount=32;
	m_bmpinfo->bmiHeader.biCompression=0;
	m_bmpinfo->bmiHeader.biSizeImage=0;
	m_bmpinfo->bmiHeader.biXPelsPerMeter=0;
	m_bmpinfo->bmiHeader.biYPelsPerMeter=0;
	m_bmpinfo->bmiHeader.biClrUsed=0;
	m_bmpinfo->bmiHeader.biClrImportant=0;

	RECT localWndRect;

	//video display window
	wnd = this->GetDlgItem(IDC_VIDEO);

	wnd->GetWindowRect(&localWndRect);

	// set the height and width to match the video window
	m_localWndWidth = localWndRect.right - localWndRect.left;
	m_localWndHeight = localWndRect.bottom - localWndRect.top;

	//get Dialog DC
	m_hdc=wnd->GetDC()->m_hDC;
	
	//initialize DIB for drawing
	hdib=DrawDibOpen();
	if(hdib!=NULL)
	{
		::DrawDibBegin(hdib,
					   m_hdc,
					   m_localWndWidth,		//don't stretch
					   m_localWndHeight,	//don't stretch
					   &m_bmpinfo->bmiHeader,
					   ZCameraManager::IMAGE_WIDTH,          //width of image
					   ZCameraManager::IMAGE_HEIGHT,         //height of image
					   0				
					   );
	
	}
}

// method used to close the DIB
void CMainDlg::closeVideoArea()
{
	// ends and closes the DIB if it was previously initialized
	if(hdib!=NULL) {
		DrawDibEnd(hdib);
		DrawDibClose(hdib);
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
	closeVideoArea();
	pUserInterfaceManager->closeApplication();
}
