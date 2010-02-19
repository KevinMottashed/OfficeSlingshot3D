// VirtualPadDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Chat.h"
#include "VirtualPadDlg.h"
#include "ChatDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVirtualPadDlg dialog


CVirtualPadDlg::CVirtualPadDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CVirtualPadDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CVirtualPadDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pParent = pParent;
	m_selectedBodyPart = RIGHT_LOWER_ARM;
	m_bTouched = false;
}


void CVirtualPadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CVirtualPadDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CVirtualPadDlg, CDialog)
	//{{AFX_MSG_MAP(CVirtualPadDlg)
	ON_WM_DESTROY()
	ON_CBN_SELCHANGE   (IDC_BODY_PART, OnSelchangeBodyPart)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVirtualPadDlg message handlers

void CVirtualPadDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
	
}

BOOL CVirtualPadDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	CChatDlg * pChatDlg = (CChatDlg*)m_pParent;

	CComboBox * pWndBodyPart = (CComboBox *)GetDlgItem(IDC_BODY_PART);
	for(int i=0; i<TOTAL_NUM_PART; i++)
		pWndBodyPart->InsertString(i, pChatDlg->m_pHugMe->getBodyPartName((tagHumanPart)i));
	pWndBodyPart->SetCurSel(m_selectedBodyPart);

	OnSelchangeBodyPart();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CVirtualPadDlg::OnSelchangeBodyPart() 
{
	// TODO: Add your control notification handler code here
	CComboBox * pWndBodyPart = (CComboBox *)GetDlgItem(IDC_BODY_PART);
	m_selectedBodyPart = (HumanPart)pWndBodyPart->GetCurSel();

	RECT wndSize;
	GetClientRect(&wndSize);

	CWnd * pWndPad = (CWnd *)GetDlgItem(IDC_PAD);
	float ratio;
	switch(m_selectedBodyPart) {
	case CHEST:
		ratio = 9.0/16.0;
		m_padLeft = 15;
		m_padWidth = wndSize.right - wndSize.left - m_padLeft*2;
		m_padHeight = (int)((float)m_padWidth*ratio);
		m_padTop = (int)((double)(wndSize.bottom - wndSize.top - m_padHeight)/2.0);
	break;
	case RIGHT_UPPER_ARM:
	case RIGHT_LOWER_ARM:
		ratio = 3.0/2.0;
		m_padTop = 50;
		m_padHeight = wndSize.bottom - wndSize.top - 100;
		m_padWidth = (int)((float)m_padHeight/ratio);
		m_padLeft = (int)((double)(wndSize.right - wndSize.left - m_padWidth)/2.0);
	break;
	default:
	break;
	}
	pWndPad->SetWindowPos(NULL, m_padLeft, m_padTop, m_padWidth, m_padHeight, SWP_SHOWWINDOW);
}

void CVirtualPadDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(nFlags == MK_LBUTTON ) {
		if( point.x >= m_padLeft && point.x <= (m_padLeft+m_padWidth) && point.y >= m_padTop && point.y <= (m_padTop+m_padHeight) ) {
			m_bTouched = true;
			m_normTouchPointX = (float)(point.x - m_padLeft)/(float)m_padWidth;
			m_normTouchPointY = (float)(m_padHeight - (point.y - m_padTop) )/(float)m_padHeight;
		} else {
			m_bTouched = false;
		}
	} else {
		m_bTouched = false;
	}

	CEdit * edit1 = (CEdit*)GetDlgItem(IDC_EDIT1);
	CEdit * edit2 = (CEdit*)GetDlgItem(IDC_EDIT2);
	CEdit * edit3 = (CEdit*)GetDlgItem(IDC_EDIT3);

	CString ed1, ed2, ed3;
	ed1.Format("%d", (int)m_bTouched);
	ed2.Format("%f", m_normTouchPointX);
	ed3.Format("%f", m_normTouchPointY);

	edit1->SetWindowText(ed1);
	edit2->SetWindowText(ed2);
	edit3->SetWindowText(ed3);
	
	CDialog::OnMouseMove(nFlags, point);
}

void CVirtualPadDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if( point.x >= m_padLeft && point.x <= (m_padLeft+m_padWidth) && point.y >= m_padTop && point.y <= (m_padTop+m_padHeight) ) {
			m_bTouched = true;
			m_normTouchPointX = (float)(point.x - m_padLeft)/(float)m_padWidth;
			m_normTouchPointY = (float)(m_padHeight - (point.y - m_padTop) )/(float)m_padHeight;
	} else {
			m_bTouched = false;
	}

	CEdit * edit1 = (CEdit*)GetDlgItem(IDC_EDIT1);
	CEdit * edit2 = (CEdit*)GetDlgItem(IDC_EDIT2);
	CEdit * edit3 = (CEdit*)GetDlgItem(IDC_EDIT3);

	CString ed1, ed2, ed3;
	ed1.Format("%d", (int)m_bTouched);
	ed2.Format("%f", m_normTouchPointX);
	ed3.Format("%f", m_normTouchPointY);

	edit1->SetWindowText(ed1);
	edit2->SetWindowText(ed2);
	edit3->SetWindowText(ed3);
	
	CDialog::OnLButtonDown(nFlags, point);
}

void CVirtualPadDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	m_bTouched = false;


	CEdit * edit1 = (CEdit*)GetDlgItem(IDC_EDIT1);
	CEdit * edit2 = (CEdit*)GetDlgItem(IDC_EDIT2);
	CEdit * edit3 = (CEdit*)GetDlgItem(IDC_EDIT3);

	CString ed1, ed2, ed3;
	ed1.Format("%d", (int)m_bTouched);
	ed2.Format("%f", m_normTouchPointX);
	ed3.Format("%f", m_normTouchPointY);

	edit1->SetWindowText(ed1);
	edit2->SetWindowText(ed2);
	edit3->SetWindowText(ed3);
	
	CDialog::OnLButtonUp(nFlags, point);
}
