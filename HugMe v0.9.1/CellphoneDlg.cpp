// CellphoneDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Chat.h"
#include "ChatDlg.h"
#include "CellphoneDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCellphoneDlg dialog

CCellphoneDlg::CCellphoneDlg(CWnd* pParent /*=NULL*/)
: CDialog(CCellphoneDlg::IDD, pParent), m_bIsListening(false), m_bIsConnected(false),
m_bTouched(false), m_selectedBodyPart(CHEST), m_normTouchPointX(0.0), m_normTouchPointY(0.0)
{
	//{{AFX_DATA_INIT(CCellphoneDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pChatDlg = pParent;
	m_pCellphoneSocket = NULL;
}


void CCellphoneDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCellphoneDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCellphoneDlg, CDialog)
	//{{AFX_MSG_MAP(CCellphoneDlg)
	ON_MESSAGE(WM_ON_ACCEPT_CELL, OnAccept)
	ON_MESSAGE(WM_ON_CLOSE_CELL, OnCloseSocket)
	ON_MESSAGE(WM_ON_RECEIVE_CELL, OnReceive)
	ON_WM_CLOSE()
	ON_BN_CLICKED(ID_YES, OnYes)
	ON_BN_CLICKED(ID_NO, OnNo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCellphoneDlg message handlers

BOOL CCellphoneDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_device = &((CChatDlg*)m_pChatDlg)->m_pHugMe->getDevice();

	CEdit* status = (CEdit*)GetDlgItem(IDC_STATUS);
	CButton* ok = (CButton*)GetDlgItem(ID_YES);
	CButton* cancel = (CButton*)GetDlgItem(ID_NO);

	status->SetWindowText("Listen to a cellphone?");
	ok->SetWindowText("Listen");

	ok->ShowWindow(TRUE);
	cancel->ShowWindow(FALSE);

	m_bIsListening = false;
	m_bIsConnected = false;
		
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

LRESULT CCellphoneDlg::OnAccept(WPARAM wParam, LPARAM lParam)
{
	m_device->m_bOnInputDevice = true;
	m_device->m_typeOnInputDevice = CELLPHONE_TOUCHPAD;

	CEdit* status = (CEdit*)GetDlgItem(IDC_STATUS);
	CButton* ok = (CButton*)GetDlgItem(ID_YES);
	CButton* cancel = (CButton*)GetDlgItem(ID_NO);

	status->SetWindowText("Cellphone is connected");
	ok->ShowWindow(FALSE);
	cancel->SetWindowText("Disconnect");
	cancel->ShowWindow(TRUE);

	m_bIsListening = false;
	m_bIsConnected = true;

	return 0;
}

LRESULT CCellphoneDlg::OnCloseSocket(WPARAM wParam, LPARAM lParam)
{
	OnNo();

	return 0;
}

LRESULT CCellphoneDlg::OnReceive(WPARAM wParam, LPARAM lParam)
{
	CellphoneContact m_contact;
	memcpy(&m_contact, (char*)wParam, sizeof(CellphoneContact));

	m_bTouched = m_contact.m_bIsContacted;
	m_selectedBodyPart = (HumanPart)m_contact.m_part;
	// Window coordinate to tactile array coordinate mappting.
	// As for chest part, we grab the cellphone wide.
	if(m_selectedBodyPart == CHEST) {
		m_normTouchPointX = m_contact.m_y;
		m_normTouchPointY = m_contact.m_x;
	} else {
		m_normTouchPointX = m_contact.m_x;
		m_normTouchPointY = 1.0 - m_contact.m_y;
	}

	CEdit * edit1 = (CEdit*)GetDlgItem(IDC_EDIT1_CELL);
	CEdit * edit2 = (CEdit*)GetDlgItem(IDC_EDIT2_CELL);
	CEdit * edit3 = (CEdit*)GetDlgItem(IDC_EDIT3_CELL);
	CEdit * edit4 = (CEdit*)GetDlgItem(IDC_EDIT4_CELL);

	CString ed1, ed2, ed3, ed4;
	ed1.Format("%d", (int)m_bTouched);
	ed2.Format("%f", m_normTouchPointX);
	ed3.Format("%f", m_normTouchPointY);
	ed4.Format("%d", m_selectedBodyPart);

	edit1->SetWindowText(ed1);
	edit2->SetWindowText(ed2);
	edit3->SetWindowText(ed3);
	edit4->SetWindowText(ed4);

	return 0;
}

void CCellphoneDlg::OnYes() 
{
	// TODO: Add extra validation here
	m_pCellphoneSocket = new CellphoneSocket();

	if( !m_pCellphoneSocket->Create(CELLPHONE_PORT) ) {
		MessageBox("Failed to create the chatting socket");
		delete m_pCellphoneSocket;
		m_pCellphoneSocket = NULL;
	}
	if( !m_pCellphoneSocket->Listen() ) {
		MessageBox("Failed to listen to a connection for chat");
		delete m_pCellphoneSocket;
		m_pCellphoneSocket = NULL;
	}

	CEdit* status = (CEdit*)GetDlgItem(IDC_STATUS);
	CButton* ok = (CButton*)GetDlgItem(ID_YES);
	CButton* cancel = (CButton*)GetDlgItem(ID_NO);

	status->SetWindowText("Listening to a cellphone...");
	ok->ShowWindow(FALSE);
	cancel->SetWindowText("Cancel");
	cancel->ShowWindow(TRUE);

	m_bIsListening = true;
	m_bIsConnected = false;
}

void CCellphoneDlg::OnNo() 
{
	// TODO: Add extra cleanup here
	CEdit* status = (CEdit*)GetDlgItem(IDC_STATUS);
	CButton* ok = (CButton*)GetDlgItem(ID_YES);
	CButton* cancel = (CButton*)GetDlgItem(ID_NO);

	if(m_pCellphoneSocket) {
		delete m_pCellphoneSocket;
		m_pCellphoneSocket = NULL;
	}

	status->SetWindowText("Listen to a cellphone?");
	ok->SetWindowText("Listen");

	ok->ShowWindow(TRUE);
	cancel->ShowWindow(FALSE);

	m_bIsListening = false;
	m_bIsConnected = false;
}

void CCellphoneDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	ShowWindow(FALSE);

	CDialog::OnClose();
}
