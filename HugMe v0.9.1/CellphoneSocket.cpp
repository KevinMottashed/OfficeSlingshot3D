// ellphoneSocket.cpp : implementation file
//

#include "stdafx.h"
#include "Chat.h"
#include "CellphoneSocket.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CWnd* CellphoneSocket::pWndMsgProc = NULL;

/////////////////////////////////////////////////////////////////////////////
// CellphoneSocket

CellphoneSocket::CellphoneSocket():m_pParent(NULL), m_pClient(NULL)
{
}

CellphoneSocket::~CellphoneSocket()
{
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CellphoneSocket, CSocket)
	//{{AFX_MSG_MAP(CellphoneSocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CellphoneSocket member functions

void CellphoneSocket::OnAccept(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	m_pClient = new CellphoneSocket();
	m_pClient->m_pParent = this;
	Accept(*m_pClient);

	if (pWndMsgProc)
	{
		pWndMsgProc->SendMessage(WM_ON_ACCEPT_CELL, NULL, NULL);
	}	
	CSocket::OnAccept(nErrorCode);
}

void CellphoneSocket::OnClose(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pWndMsgProc)
	{
		pWndMsgProc->SendMessage(WM_ON_CLOSE_CELL, NULL, NULL);
	}
	
	CSocket::OnClose(nErrorCode);
}

void CellphoneSocket::OnReceive(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	// Data structure from the cellphone
	// It's string like "x,x,x.xx,x.xx"
	// each value seperated by comma represents the values for CellphoneContact.
	char buff[13];
	int ret = Receive(buff, 14);
	char first;
	char second;
	char third[5];
	char fourth[5];

	first = buff[0];
	second = buff[2];
	memcpy(third, &buff[4], 4);
	memcpy(fourth, &buff[9], 4);

	// adding terminating character
	third[4] = '\0';
	fourth[4] = '\0';

	
	m_cellphoneContact.m_bIsContacted = atoi(&first) != 0; // != 0 converts to bool without a compiler warning
	m_cellphoneContact.m_part = atoi(&second);
	m_cellphoneContact.m_x = atof(third);
	m_cellphoneContact.m_y = atof(fourth);

	switch (ret) {
	case 0:
		Close();
		break;

	case SOCKET_ERROR:
		if (GetLastError() != WSAEWOULDBLOCK)
		{
			Close();
		}
		break;

	default:
		break;
	}

	if (pWndMsgProc) {
		pWndMsgProc->SendMessage(WM_ON_RECEIVE_CELL, (WPARAM) &m_cellphoneContact, NULL);
	}
	
	CSocket::OnReceive(nErrorCode);
}
