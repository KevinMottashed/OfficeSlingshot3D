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
	ON_WM_DESTROY()
	ON_EN_CHANGE(IDC_CHAT_INPUT, OnChangeChatInput)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainDlg message handlers

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
		MessageBox(lookup(status).c_str());
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
	} else {
		MessageBox(lookup(status).c_str());
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
	} else {
		MessageBox(lookup(status).c_str());
	}
	
}

LRESULT CMainDlg::OnNetworkEstablished(WPARAM wParam)
{
	string* remoteUserName = (string*) wParam;
	ostringstream os;
	os << *remoteUserName << " has joined the game";
	MessageBox(os.str().c_str());

	CMenu* pMenu = GetMenu();
	pMenu->EnableMenuItem(ID_GAME_STARTGAME, MF_ENABLED | MF_BYCOMMAND);

	return 0;
}

LRESULT CMainDlg::OnNetworkDisconnected(WPARAM wParam)
{
	string* remoteUserName = (string*) wParam;
	ostringstream os;
	os << *remoteUserName << " has disconnected from the game";
	MessageBox(os.str().c_str());

	CMenu* pMenu = GetMenu();
	pMenu->EnableMenuItem(ID_NETWORK_CONNECT, MF_ENABLED | MF_BYCOMMAND);
	pMenu->EnableMenuItem(ID_NETWORK_LISTEN, MF_ENABLED | MF_BYCOMMAND);
	pMenu->EnableMenuItem(ID_NETWORK_DISCONNECT, MF_GRAYED | MF_BYCOMMAND);

	pMenu->EnableMenuItem(ID_GAME_STARTGAME, MF_GRAYED | MF_BYCOMMAND);
	pMenu->EnableMenuItem(ID_GAME_EXITGAME, MF_GRAYED | MF_BYCOMMAND);

	return 0;
}

LRESULT CMainDlg::OnNetworkError(WPARAM wParam)
{
	rc_network* error = (rc_network*) wParam;
	MessageBox(lookup(*error).c_str());

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
}

LRESULT CMainDlg::OnGameStarted(WPARAM wParam)
{
	string* remoteUserName = (string*) wParam;
	ostringstream os;
	os << *remoteUserName << " has started the game";
	MessageBox(os.str().c_str());

	CMenu* pMenu = GetMenu();
	pMenu->EnableMenuItem(ID_GAME_STARTGAME, MF_GRAYED | MF_BYCOMMAND);
	pMenu->EnableMenuItem(ID_GAME_EXITGAME, MF_ENABLED | MF_BYCOMMAND);

	return 0;
}

LRESULT CMainDlg::OnGameExited(WPARAM wParam)
{
	string* remoteUserName = (string*) wParam;
	ostringstream os;
	os << *remoteUserName << " has exited the game";
	MessageBox(os.str().c_str());

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
		pUserInterfaceManager->sendChatButtonPushed((string)chatInput);

		m_editChatInput.SetWindowText("");
	}
}

LRESULT CMainDlg::OnNewChatMessage(WPARAM wParam, LPARAM lParam) 
{
	string* remoteUserName = (string*) wParam;
	string* message = (string*) lParam;
	ostringstream os;
	os << *remoteUserName << " : " << *message << "\n";

	CString oldText;
	m_richChat.GetWindowText(oldText);

	m_richChat.SetWindowText(os.str().c_str() + oldText);

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

void CMainDlg::OnDestroy() 
{
	pUserInterfaceManager->closeApplication();
}
