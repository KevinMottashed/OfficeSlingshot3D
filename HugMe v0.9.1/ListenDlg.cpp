// CListenDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Chat.h"
#include "ListenDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CListenDlg dialog


CListenDlg::CListenDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CListenDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CListenDlg)
	m_localName =("");
	//}}AFX_DATA_INIT
}


void CListenDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CListenDlg)
	DDX_Text(pDX, IDC_LOCALNAME, m_localName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CListenDlg, CDialog)
	//{{AFX_MSG_MAP(CListenDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CListenDlg message handlers

