// MainDlg.cpp : implementation file
//

#include "stdafx.h"
#include "chat.h"
#include "MainDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainDlg dialog


CMainDlg::CMainDlg(UserInterfaceManager* m_pUserInterfaceManager, CWnd* pParent /*=NULL*/)
	: pUserInterfaceManager(pUserInterfaceManager), CDialog(CMainDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMainDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMainDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMainDlg, CDialog)
	//{{AFX_MSG_MAP(CMainDlg)
	ON_COMMAND(ID_NETWORK_CONNECT, OnNetworkConnect)
	ON_COMMAND(ID_NETWORK_DISCONNECT, OnNetworkDisconnect)
	ON_COMMAND(ID_NETWORK_LISTEN, OnNetworkListen)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainDlg message handlers

void CMainDlg::OnNetworkConnect() 
{
	// TODO: Add your command handler code here
	rc_network status = pUserInterfaceManager->networkConnectButtonPushed("Connector", "127.0.0.1");

	MessageBox(lookup(status).c_str());

	if (status == SUCCESS){
		MessageBox("Connected !");
	}
	
}

void CMainDlg::OnNetworkDisconnect() 
{
	// TODO: Add your command handler code here
	rc_network status = pUserInterfaceManager->networkDisconnectButtonPushed();

	MessageBox(lookup(status).c_str());

	if (status == SUCCESS){
		MessageBox("Disonnected !");
	}
	
}

void CMainDlg::OnNetworkListen() 
{
	// TODO: Add your command handler code here
	rc_network status = pUserInterfaceManager->networkListenButtonPushed("Listener");

	MessageBox(lookup(status).c_str());

	if (status == SUCCESS){
		MessageBox("Listening !");
	}
	
}
