// StaticCanvas.cpp : implementation file
//

#include "stdafx.h"
#include "Chat.h"
#include "StaticCanvas.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStaticCanvas

CStaticCanvas::CStaticCanvas()
{
}

CStaticCanvas::~CStaticCanvas()
{
}


BEGIN_MESSAGE_MAP(CStaticCanvas, CStatic)
	//{{AFX_MSG_MAP(CStaticCanvas)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStaticCanvas message handlers

void CStaticCanvas::OnPaint() 
{
	const int halfOfSize = 5;

	CPaintDC dc(this); // device context for painting
	
	CRect rect;
	GetClientRect(&rect);
	dc.FillRect(&rect, CBrush::FromHandle((HBRUSH) GetStockObject(WHITE_BRUSH)));

	if (m_pDrawingPattern == NULL)
		return;

	Pattern& ptn = *m_pDrawingPattern;
	for (unsigned int i = 0; i < ptn.points.size(); i++)
	{
		if (i > (unsigned int)ptn.currentPoint)
			break;

		if (ptn.points[i].intensity > 0)
		{
			// 강도1 -> 회색, 강도10 -> 검은색
			int int255 = 255 - (((ptn.points[i].intensity - 1) * 200 / 9) + 55);
			CBrush rectBrush(RGB(int255, int255, int255));
			CBrush* oldBrush;
			oldBrush = dc.SelectObject(&rectBrush);

			int x = (int) (ptn.points[i].x * rect.Width());
			int y = (int) ((1.0 - ptn.points[i].y) * rect.Height());
			dc.SetBkColor(RGB(255, 0, 0));
			dc.Ellipse(x - halfOfSize, y - halfOfSize, x + halfOfSize, y + halfOfSize);
			dc.SelectObject(oldBrush);
		}
		if ((i == ptn.currentPoint) || (i == ptn.points.size() - 1 && ptn.currentPoint == ptn.points.size()))
		{
			int x = (int) (ptn.points[i].x * rect.Width());
			int y = (int) ((1.0 - ptn.points[i].y) * rect.Height());
			CPen pen(PS_SOLID, 1, RGB(255, 0, 0));
			dc.SelectObject(&pen);
			dc.MoveTo(x - halfOfSize, y);
			dc.LineTo(x + halfOfSize, y);
			dc.MoveTo(x, y - halfOfSize);
			dc.LineTo(x, y + halfOfSize);
		}
	}
}
