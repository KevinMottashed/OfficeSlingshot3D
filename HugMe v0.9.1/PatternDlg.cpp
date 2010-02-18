// PatternDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Chat.h"
#include "PatternDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPatternDlg dialog


CPatternDlg::CPatternDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPatternDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPatternDlg)
	m_strIntensity = _T("");
	m_strFrame = _T("");
	m_strName = _T("");
	//}}AFX_DATA_INIT
}


void CPatternDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPatternDlg)
	DDX_Control(pDX, IDC_STATIC_ICON, m_picIcon);
	DDX_Control(pDX, IDC_PIC_PATTERN, m_picPattern);
	DDX_Control(pDX, IDC_SCROLLBAR_INTENSITY, m_scrollIntensity);
	DDX_Control(pDX, IDC_SCROLLBAR_FRAME, m_scrollFrame);
	DDX_Text(pDX, IDC_STATIC_INTENSITY, m_strIntensity);
	DDX_Text(pDX, IDC_STATIC_FRAME, m_strFrame);
	DDX_Text(pDX, IDC_EDIT_NAME, m_strName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPatternDlg, CDialog)
	//{{AFX_MSG_MAP(CPatternDlg)
	ON_BN_CLICKED(IDC_BUTTON_DELETE_ALL, OnButtonDeleteAll)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, OnButtonClose)
	ON_WM_HSCROLL()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_BUTTON_LOADICO, OnButtonLoadico)
	ON_EN_CHANGE(IDC_EDIT_NAME, OnChangeEditName)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPatternDlg message handlers

BOOL CPatternDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	SCROLLINFO info;
	m_scrollFrame.GetScrollInfo(&info);
	info.nPage = 1;
	m_scrollFrame.SetScrollInfo(&info);
	m_scrollIntensity.GetScrollInfo(&info);
	info.nPage = 1;
	m_scrollIntensity.SetScrollInfo(&info);

	m_pattern.currentPoint = m_pattern.points.size();
	m_curPoint = NULL;
	m_picPattern.m_pDrawingPattern = &m_pattern;

	int frameCount = m_pattern.points.size();
	m_strFrame.Format("%d", frameCount + 1);
	m_strIntensity = "0";

	m_scrollFrame.SetScrollRange(0, frameCount, FALSE);
	m_scrollFrame.SetScrollPos(frameCount, FALSE);
	m_scrollIntensity.SetScrollRange(0, 10, FALSE);
	m_scrollIntensity.SetScrollPos(0, FALSE);

	if (frameCount > 1)
	{
		int intensity = m_pattern.points.back().intensity;
		m_strIntensity.Format("%d", intensity);
		m_scrollIntensity.SetScrollPos(intensity, FALSE);
	}

	if (m_pattern.pIconBitmap)
	{
		m_picIcon.SetBitmap(*m_pattern.pIconBitmap);
	}

	m_strName = m_pattern.name.c_str();
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPatternDlg::OnButtonDeleteAll() 
{
	m_pattern.currentPoint = 0;
	m_pattern.points.clear();
	m_scrollFrame.SetScrollRange(0, 0, FALSE);
	m_scrollFrame.SetScrollPos(0, FALSE);
	m_scrollIntensity.SetScrollPos(0, FALSE);
	m_strFrame = "1";
	m_strIntensity = "0";
	UpdateData(FALSE);
	Invalidate();
}

void CPatternDlg::OnButtonClose() 
{
	UpdateData();
	m_pattern.name = m_strName;
	if (m_pattern.pIconBitmap == NULL)
	{
		MessageBox("아이콘을 선택해 주세요.");
		return;
	}
	OnOK();
}

void CPatternDlg::OnButtonLoadico() 
{
	CFileDialog dlg(TRUE);
	if (dlg.DoModal() == IDOK)
	{
		CBitmap* pBitmap = new CBitmap();
		int iconCnt = (int) ExtractIcon(NULL, dlg.GetFileName(), -1);
		for (int i = 0; i < iconCnt; i++)
		{
			HICON hIcon = ExtractIcon(NULL, dlg.GetFileName(), i);
			ICONINFO iconInfo;
			GetIconInfo(hIcon, &iconInfo);
			DeleteObject(iconInfo.hbmMask);
			DestroyIcon(hIcon);

			BITMAP bm;
			GetObject(iconInfo.hbmColor, sizeof(bm), &bm);
			if (bm.bmWidth != 32 || bm.bmHeight != 32)
				continue;

			pBitmap->Attach(iconInfo.hbmColor);

			delete m_pattern.pIconBitmap;
			m_pattern.pIconBitmap = pBitmap;

			m_picIcon.SetBitmap(*pBitmap);
			Invalidate();
			break;
		}
	}
}

void CPatternDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	int nNewPos = pScrollBar->GetScrollPos();
	int nMin, nMax;
	pScrollBar->GetScrollRange(&nMin, &nMax);
	switch (nSBCode)
	{
	case SB_LEFT:
	case SB_LINELEFT:
	case SB_PAGELEFT:
		nNewPos--;
		break;
		
	case SB_RIGHT:
	case SB_LINERIGHT:
	case SB_PAGERIGHT:
		nNewPos++;
		break;
		
	case SB_THUMBTRACK:
	case SB_THUMBPOSITION:
		nNewPos = nPos;
		break;
		
	case SB_ENDSCROLL:
		break;
	}

	if (pScrollBar == &m_scrollFrame && nNewPos == nMax + 1)
	{
		pScrollBar->SetScrollRange(nMin, nMax + 1, FALSE);
		pScrollBar->SetScrollPos(nNewPos, FALSE);

		Pattern::Point point;
		point.x = 0;
		point.y = 0;
		point.intensity = 0;
		m_pattern.points.push_back(point);
		m_pattern.currentPoint = m_pattern.points.size();
	}
	else
	{
		// 최대, 최소 범위 안에 들어오게 함.
		if (nNewPos > nMax)
			nNewPos = nMax;
		if (nNewPos < nMin)
			nNewPos = nMin;
		
		pScrollBar->SetScrollPos(nNewPos);
		if (pScrollBar == &m_scrollFrame)
		{
			m_pattern.currentPoint = nNewPos;
			m_strFrame.Format("%d", nNewPos + 1);
		}
		else if (pScrollBar == &m_scrollIntensity)
		{
			m_strIntensity.Format("%d", nNewPos);

			Pattern& ptn = m_pattern;
			// 점의 강도를 조절한다.
			if (ptn.points.size() > (double) ptn.currentPoint)
			{
				Pattern::Point& point = m_pattern.points[ptn.currentPoint];
				point.intensity = nNewPos;
			}
			if (ptn.points.size() == ptn.currentPoint && ptn.currentPoint > 0)
			{
				Pattern::Point& point = m_pattern.points.back();
				point.intensity = nNewPos;
			}
		}
	}
	UpdateData(FALSE);
	// 패턴을 다시 그림.
	Invalidate();
	
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CPatternDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CRect rect;
	m_picPattern.GetWindowRect(&rect);
	ScreenToClient(&rect);
	if (rect.PtInRect(point))
	{
		point -= rect.TopLeft();
		if (m_pattern.currentPoint == m_pattern.points.size())
		{
			Pattern::Point pt;
			pt.x = (double) point.x / (double) rect.Width();
			pt.y = 1.0 - (double) point.y / (double) rect.Height();
			pt.intensity = m_scrollIntensity.GetScrollPos(); // 현재 강도
			m_pattern.points.push_back(pt);
			m_curPoint = &m_pattern.points.back();

			m_pattern.currentPoint = m_pattern.points.size();
			m_scrollFrame.SetScrollRange(0, m_pattern.currentPoint);
			m_scrollFrame.SetScrollPos(m_pattern.currentPoint);
			m_strFrame.Format("%d", m_pattern.currentPoint + 1);
		}
		else
		{
			ASSERT((double)m_pattern.currentPoint < m_pattern.points.size());
			Pattern::Point& pt = m_pattern.points[m_pattern.currentPoint];
			pt.x = (double) point.x / (double) rect.Width();
			pt.y = 1.0 - (double) point.y / (double) rect.Height();
			pt.intensity = m_scrollIntensity.GetScrollPos(); // 현재 강도
			m_curPoint = &pt;
		}
		point += rect.TopLeft();
		UpdateData(FALSE);
		// 패턴을 다시 그림.
		Invalidate();
		SetCapture();
	}

	CDialog::OnLButtonDown(nFlags, point);
}

void CPatternDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	m_curPoint = NULL;
	ReleaseCapture();
	CDialog::OnLButtonUp(nFlags, point);
}

void CPatternDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	CRect rect;
	m_picPattern.GetWindowRect(&rect);
	ScreenToClient(&rect);
	if (nFlags & MK_LBUTTON)
	{
		if (rect.PtInRect(point) && (m_curPoint != NULL))
		{
			point -= rect.TopLeft();
			Pattern::Point& pt = *m_curPoint;
			pt.x = (double) point.x / (double) rect.Width();
			pt.y = 1.0 - (double) point.y / (double) rect.Height();
			point += rect.TopLeft();
			// 패턴을 다시 그림.
			Invalidate();
		}
	}

	CDialog::OnMouseMove(nFlags, point);
}

void CPatternDlg::OnChangeEditName() 
{
	UpdateData();
}
