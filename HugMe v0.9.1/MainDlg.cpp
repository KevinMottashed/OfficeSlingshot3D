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

/////////////////////////////////////////////////////////////////////////////
// CMainDlg dialog


CMainDlg::CMainDlg(UserInterfaceManager* m_pUserInterfaceManager, CWnd* pParent /*=NULL*/)
	: pUserInterfaceManager(pUserInterfaceManager), CDialog(CMainDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMainDlg)
		string line;
		ifstream myfile ("userPreferences.txt");
		if (myfile.is_open()) {
			getline (myfile, line);
			m_userName = line;
			getline (myfile, line);
			m_ipAddress = line;
			myfile.close();
		} else {
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

BOOL CMainDlg::OnInitDialog()
{
	return CDialog::OnInitDialog();
}

void CMainDlg::OnNetworkConnect() 
{
	rc_network status = pUserInterfaceManager->networkConnectButtonPushed(m_ipAddress.c_str(), m_userName);

	if (status == SUCCESS){
		CMenu* pMenu = GetMenu();
		pMenu->EnableMenuItem(ID_NETWORK_CONNECT, MF_GRAYED | MF_BYCOMMAND);
		pMenu->EnableMenuItem(ID_NETWORK_LISTEN, MF_GRAYED | MF_BYCOMMAND);
		pMenu->EnableMenuItem(ID_NETWORK_DISCONNECT, MF_ENABLED | MF_BYCOMMAND);

		pMenu->EnableMenuItem(ID_GAME_STARTGAME, MF_ENABLED | MF_BYCOMMAND);
	} else {
		AddChatContent(lookup(status).c_str());
	}
	
}

void CMainDlg::OnNetworkDisconnect() 
{
	rc_network status = pUserInterfaceManager->networkDisconnectButtonPushed();

	if (status == SUCCESS){
		CMenu* pMenu = GetMenu();
		pMenu->EnableMenuItem(ID_NETWORK_CONNECT, MF_ENABLED | MF_BYCOMMAND);
		pMenu->EnableMenuItem(ID_NETWORK_LISTEN, MF_ENABLED | MF_BYCOMMAND);
		pMenu->EnableMenuItem(ID_NETWORK_DISCONNECT, MF_GRAYED | MF_BYCOMMAND);

		pMenu->EnableMenuItem(ID_GAME_STARTGAME, MF_GRAYED | MF_BYCOMMAND);
		pMenu->EnableMenuItem(ID_GAME_EXITGAME, MF_GRAYED | MF_BYCOMMAND);

		AddChatContent("Disconnected");
	} else {
		AddChatContent(lookup(status).c_str());
	}
	
}

void CMainDlg::OnNetworkListen() 
{
	rc_network status = pUserInterfaceManager->networkListenButtonPushed(m_userName);

	if (status == SUCCESS){
		CMenu* pMenu = GetMenu();
		pMenu->EnableMenuItem(ID_NETWORK_CONNECT, MF_GRAYED | MF_BYCOMMAND);
		pMenu->EnableMenuItem(ID_NETWORK_LISTEN, MF_GRAYED | MF_BYCOMMAND);
		pMenu->EnableMenuItem(ID_NETWORK_DISCONNECT, MF_ENABLED | MF_BYCOMMAND);

		AddChatContent("Listening");
	} else {
		AddChatContent(lookup(status).c_str());
	}
	
}

LRESULT CMainDlg::OnNetworkEstablished(WPARAM wParam, LPARAM lParam)
{
	string* remoteUserName = (string*) wParam;
	ostringstream os;
	os << *remoteUserName << " has joined the game";

	AddChatContent(os.str().c_str());

	CMenu* pMenu = GetMenu();
	pMenu->EnableMenuItem(ID_GAME_STARTGAME, MF_ENABLED | MF_BYCOMMAND);

	return 0;
}

LRESULT CMainDlg::OnNetworkDisconnected(WPARAM wParam, LPARAM lParam)
{
	string* remoteUserName = (string*) wParam;
	ostringstream os;
	os << *remoteUserName << " has disconnected from the game";

	AddChatContent(os.str().c_str());

	CMenu* pMenu = GetMenu();
	pMenu->EnableMenuItem(ID_NETWORK_CONNECT, MF_ENABLED | MF_BYCOMMAND);
	pMenu->EnableMenuItem(ID_NETWORK_LISTEN, MF_ENABLED | MF_BYCOMMAND);
	pMenu->EnableMenuItem(ID_NETWORK_DISCONNECT, MF_GRAYED | MF_BYCOMMAND);

	pMenu->EnableMenuItem(ID_GAME_STARTGAME, MF_GRAYED | MF_BYCOMMAND);
	pMenu->EnableMenuItem(ID_GAME_EXITGAME, MF_GRAYED | MF_BYCOMMAND);

	return 0;
}

LRESULT CMainDlg::OnNetworkError(WPARAM wParam, LPARAM lParam)
{
	rc_network* error = (rc_network*) wParam;

	AddChatContent(lookup(*error).c_str());

	CMenu* pMenu = GetMenu();
	pMenu->EnableMenuItem(ID_NETWORK_CONNECT, MF_ENABLED | MF_BYCOMMAND);
	pMenu->EnableMenuItem(ID_NETWORK_LISTEN, MF_ENABLED | MF_BYCOMMAND);
	pMenu->EnableMenuItem(ID_NETWORK_DISCONNECT, MF_GRAYED | MF_BYCOMMAND);

	pMenu->EnableMenuItem(ID_GAME_STARTGAME, MF_GRAYED | MF_BYCOMMAND);
	pMenu->EnableMenuItem(ID_GAME_EXITGAME, MF_GRAYED | MF_BYCOMMAND);

	return 0;
}

void CMainDlg::OnPreferencesEdit() 
{
	CPreferences prefs;

	if(prefs.DoModal() == IDOK) {
		m_userName = prefs.m_userName;
		m_ipAddress = prefs.m_strAddress;

		ofstream myfile;
		myfile.open ("userPreferences.txt");
		myfile << (string)prefs.m_userName << "\n";
		myfile << (string)prefs.m_strAddress;
		myfile.close();
	}
	
}

void CMainDlg::OnStartGame()
{
	pUserInterfaceManager->startGameButtonPushed();

	CMenu* pMenu = GetMenu();
	pMenu->EnableMenuItem(ID_NETWORK_CONNECT, MF_GRAYED | MF_BYCOMMAND);
	pMenu->EnableMenuItem(ID_NETWORK_LISTEN, MF_GRAYED | MF_BYCOMMAND);
	pMenu->EnableMenuItem(ID_NETWORK_DISCONNECT, MF_ENABLED | MF_BYCOMMAND);

	pMenu->EnableMenuItem(ID_GAME_STARTGAME, MF_GRAYED | MF_BYCOMMAND);
	pMenu->EnableMenuItem(ID_GAME_EXITGAME, MF_ENABLED | MF_BYCOMMAND);

	AddChatContent("Started the game");
}

void CMainDlg::OnExitGame() 
{
	pUserInterfaceManager->exitGameButtonPushed();

	CMenu* pMenu = GetMenu();
	pMenu->EnableMenuItem(ID_NETWORK_CONNECT, MF_GRAYED | MF_BYCOMMAND);
	pMenu->EnableMenuItem(ID_NETWORK_LISTEN, MF_GRAYED | MF_BYCOMMAND);
	pMenu->EnableMenuItem(ID_NETWORK_DISCONNECT, MF_ENABLED | MF_BYCOMMAND);

	pMenu->EnableMenuItem(ID_GAME_STARTGAME, MF_ENABLED | MF_BYCOMMAND);
	pMenu->EnableMenuItem(ID_GAME_EXITGAME, MF_GRAYED | MF_BYCOMMAND);

	AddChatContent("Exited the game");
}

LRESULT CMainDlg::OnGameStarted(WPARAM wParam, LPARAM lParam)
{
	string* remoteUserName = (string*) wParam;
	ostringstream os;
	os << *remoteUserName << " has started the game";

	AddChatContent(os.str().c_str());

	CMenu* pMenu = GetMenu();
	pMenu->EnableMenuItem(ID_GAME_STARTGAME, MF_GRAYED | MF_BYCOMMAND);
	pMenu->EnableMenuItem(ID_GAME_EXITGAME, MF_ENABLED | MF_BYCOMMAND);

	return 0;
}

LRESULT CMainDlg::OnGameExited(WPARAM wParam, LPARAM lParam)
{
	string* remoteUserName = (string*) wParam;
	ostringstream os;
	os << *remoteUserName << " has exited the game";

	AddChatContent(os.str().c_str());

	CMenu* pMenu = GetMenu();
	pMenu->EnableMenuItem(ID_GAME_STARTGAME, MF_ENABLED | MF_BYCOMMAND);
	pMenu->EnableMenuItem(ID_GAME_EXITGAME, MF_GRAYED | MF_BYCOMMAND);

	return 0;
}

void CMainDlg::OnSendChat() 
{
	CString chatInput;
	m_editChatInput.GetWindowText(chatInput);

	if(!chatInput.IsEmpty()) {
		rc_network status = pUserInterfaceManager->sendChatButtonPushed((string)chatInput);

		if (status == SUCCESS){
			m_editChatInput.SetWindowText("");

			ostringstream os;
			os << m_userName << " : " << (string)chatInput;

			AddChatContent(os.str().c_str());
		} else {
			AddChatContent(lookup(status).c_str());
		}
	}
}

LRESULT CMainDlg::OnNewChatMessage(WPARAM wParam, LPARAM lParam) 
{
	string* remoteUserName = (string*) wParam;
	string* message = (string*) lParam;
	ostringstream os;
	os << *remoteUserName << " : " << *message;

	AddChatContent(os.str().c_str());

	m_editChatInput.SetWindowText("");

	return 0;
}

void CMainDlg::OnChangeChatInput()
{
	CString newChatInput;
	m_editChatInput.GetWindowText(newChatInput);

	if(!newChatInput.IsEmpty()) {
		m_sendChatButton.EnableWindow(TRUE);
	} else {
		m_sendChatButton.EnableWindow(FALSE);
	}
}

LRESULT CMainDlg::OnDisplayNewFrame(WPARAM wParam, LPARAM lParam)
{
	char* vRGB = (char*) wParam;

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
				  320,				 // src : width
				  240,				 // src : height
				  DDF_SAME_DRAW			 // use prev params....
				  );

	return 0;
}
void CMainDlg::initVideoArea()
{
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


	//Adjust display windows
	RECT localWndRect;

	wnd = this->GetDlgItem(IDC_VIDEO);	 //Video display window
	wnd->GetWindowRect(&localWndRect);

	m_localWndWidth = localWndRect.right - localWndRect.left;
	m_localWndHeight = localWndRect.bottom - localWndRect.top;

	//Get Dialog DC
	m_hdc=wnd->GetDC()->m_hDC;
	
	//Initialize DIB for drawing...
	hdib=DrawDibOpen();
	if(hdib!=NULL)
	{
		::DrawDibBegin(hdib,
					   m_hdc,
					   m_localWndWidth,		//don't stretch
					   m_localWndHeight,	//don't stretch
					   &m_bmpinfo->bmiHeader,
					   320,          //width of image
					   240,         //height of image
					   0				
					   );
	
	}
}

void CMainDlg::closeVideoArea()
{
	if(hdib!=NULL) {
		DrawDibEnd(hdib);
		DrawDibClose(hdib);
	}
}

void CMainDlg::AddChatContent(CString strCont)
{
	CString str;
	int oldLineCnt = m_richChat.GetLineCount();
	m_richChat.SetSel(m_richChat.GetWindowTextLength(), -1);
	m_richChat.ReplaceSel(strCont + "\r\n");
	int newLineCnt = m_richChat.GetLineCount();
	m_richChat.LineScroll(newLineCnt - oldLineCnt);
}

BOOL CMainDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_ESCAPE)
		{
			//MessageBox("Escape");
			// TODO Perform Escape action
			return TRUE;
		}
			
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CMainDlg::OnDestroy() 
{
	pUserInterfaceManager->closeApplication();
}
