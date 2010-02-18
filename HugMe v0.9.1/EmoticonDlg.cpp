// EmoticonDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Chat.h"
#include "EmoticonDlg.h"
#include "PatternDlg.h"
#include "ChatDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Utility sub-routines

static CString GetExeDirectory()
{
	char exePath[260];
	int index;
	index = (int) GetModuleFileName(GetModuleHandle(NULL), exePath, 260) - 1;
	while (index >= 0)
	{
		char ch = exePath[index];
		exePath[index] = 0;
		index--;
		if (ch == '\\')
			break;
	}
	return CString(exePath);
}


/////////////////////////////////////////////////////////////////////////////
// CEmoticonDlg dialog


CEmoticonDlg::CEmoticonDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEmoticonDlg::IDD, pParent)
	, m_bHideWhenInactive(TRUE)
{
	//{{AFX_DATA_INIT(CEmoticonDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CEmoticonDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEmoticonDlg)
	DDX_Control(pDX, IDC_LIST_EMOTICON, m_listEmoticon);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEmoticonDlg, CDialog)
	//{{AFX_MSG_MAP(CEmoticonDlg)
	ON_WM_ACTIVATE()
	ON_BN_CLICKED(IDC_BUTTON_NEW, OnButtonNew)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, OnButtonClose)
	ON_BN_CLICKED(IDC_BUTTON_EDIT, OnButtonEdit)
	ON_BN_CLICKED(IDC_BUTTON_SEND, OnButtonSend)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnButtonDelete)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEmoticonDlg message handlers

BOOL CEmoticonDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CImageList* pImageList = new CImageList();
	pImageList->Create(32, 32, ILC_COLOR16, 0, 1);
	m_listEmoticon.SetImageList(pImageList, LVSIL_NORMAL);
	LoadEmoticons();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CEmoticonDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialog::OnActivate(nState, pWndOther, bMinimized);
	if (nState == WA_INACTIVE && m_bHideWhenInactive == TRUE)
		PostMessage(WM_COMMAND, IDCANCEL);
}

void CEmoticonDlg::PostNcDestroy() 
{
	CDialog::PostNcDestroy();
	delete this;
}

void CEmoticonDlg::OnOK() 
{
	ShowWindow(SW_HIDE);
}

void CEmoticonDlg::OnCancel()
{
	ShowWindow(SW_HIDE);
}

void CEmoticonDlg::OnButtonNew() 
{
	m_bHideWhenInactive = FALSE;
	CPatternDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		CImageList* pImageList = m_listEmoticon.GetImageList(LVSIL_NORMAL);
		Pattern* pPattern = new Pattern();
		*pPattern = dlg.m_pattern;

		LVITEM item = {0};
		item.mask = LVIF_IMAGE | LVIF_PARAM;
		item.iItem = m_listEmoticon.GetItemCount();
		item.iImage = pImageList->Add(pPattern->pIconBitmap, RGB(0, 0, 0));
		item.lParam = (LPARAM) pPattern;
		m_listEmoticon.InsertItem(&item);
	}
	m_bHideWhenInactive = TRUE;
	SaveEmoticons();
	SetFocus();
}

void CEmoticonDlg::OnButtonClose() 
{
	OnOK();
}

void CEmoticonDlg::OnButtonEdit() 
{
	m_bHideWhenInactive = FALSE;
	if (m_listEmoticon.GetSelectedCount() == 0)
	{
		MessageBox("select emoticon!");
	}
	else
	{
		POSITION pos = m_listEmoticon.GetFirstSelectedItemPosition();
		if (pos)
		{
			LVITEM item = {0};
			item.iItem = m_listEmoticon.GetNextSelectedItem(pos);
			item.mask = LVIF_PARAM;
			m_listEmoticon.GetItem(&item);
			
			Pattern* pPattern = (Pattern*) item.lParam;
			CPatternDlg dlg;
			dlg.m_pattern = *pPattern;
			if (dlg.DoModal() == IDOK)
			{
				CImageList* pImageList = m_listEmoticon.GetImageList(LVSIL_NORMAL);
				pImageList->Replace(item.iImage, pPattern->pIconBitmap, NULL);
				*pPattern = dlg.m_pattern;
			}
		}
	}
	m_bHideWhenInactive = TRUE;
	SaveEmoticons();
	SetFocus();
}

void CEmoticonDlg::OnButtonSend() 
{
	m_bHideWhenInactive = FALSE;
	if (m_listEmoticon.GetSelectedCount() == 0)
	{
		MessageBox("select emoticon!");
	}
	else
	{
		POSITION pos = m_listEmoticon.GetFirstSelectedItemPosition();
		if (pos)
		{
			LVITEM item = {0};
			item.iItem = m_listEmoticon.GetNextSelectedItem(pos);
			item.mask = LVIF_PARAM;
			m_listEmoticon.GetItem(&item);

			CChatDlg* pParent = (CChatDlg*) this->GetParent();
			pParent->SendEmoticon((Pattern*) item.lParam);
		}
	}
	m_bHideWhenInactive = TRUE;
}

void CEmoticonDlg::OnButtonDelete() 
{
	m_bHideWhenInactive = FALSE;
	if (m_listEmoticon.GetSelectedCount() == 0)
	{
		MessageBox("select emoticon!");
	}
	else
	{
		POSITION pos = m_listEmoticon.GetFirstSelectedItemPosition();
		if (pos)
		{
			LVITEM item = {0};
			item.iItem = m_listEmoticon.GetNextSelectedItem(pos);
			item.mask = LVIF_PARAM;
			m_listEmoticon.GetItem(&item);
			delete (Pattern*) item.lParam;
			m_listEmoticon.DeleteItem(item.iItem);
		}
	}
	SaveEmoticons();
	m_bHideWhenInactive = TRUE;
}

void CEmoticonDlg::LoadEmoticons()
{
	FILE* fp = fopen(GetExeDirectory() + "\\hapticon.dat", "rb");
	if (fp == NULL)
		return;

	CImageList* pImageList = m_listEmoticon.GetImageList(LVSIL_NORMAL);
	while (true)
	{
		char name[32];
		fread(name, 1, 32, fp);
		name[31] = 0;

		if (feof(fp))
			break; // end of file

		PatternPacket packet;
		Pattern* pPattern = new Pattern();
		pPattern->name = name;

		fread(&packet, 1, sizeof(packet), fp);
		BYTE* pPoints = new BYTE[packet.dwPointsBytes];
		BYTE* pIcon = new BYTE[packet.dwIconBytes];
		fread(pPoints, 1, packet.dwPointsBytes, fp);
		fread(pIcon, 1, packet.dwIconBytes, fp);
		pPattern->samplingTime = packet.samplingTime;
		pPattern->points.reserve(packet.nPointCount);
		pPattern->points.insert(pPattern->points.begin(), (Pattern::Point*) pPoints, (Pattern::Point*) (pPoints + packet.dwPointsBytes));
		pPattern->pIconBitmap = CBitmap::FromHandle(CreateBitmap(32, 32, 1, packet.dwIconBytes / (32 * 32) * 8, pIcon));
		pPattern->currentPoint = packet.nPointCount;

		delete [] pPoints;
		delete [] pIcon;

		LVITEM item = {0};
		item.mask = LVIF_IMAGE | LVIF_PARAM;
		item.iItem = m_listEmoticon.GetItemCount();
		item.iImage = pImageList->Add(pPattern->pIconBitmap, RGB(0, 0, 0));
		item.lParam = (LPARAM) pPattern;
		m_listEmoticon.InsertItem(&item);
	}
	fclose(fp);
}

void CEmoticonDlg::SaveEmoticons()
{
	FILE* fp = fopen(GetExeDirectory() + "\\hapticon.dat", "wb");
	if (fp == NULL)
		return;

	CImageList* pImageList = m_listEmoticon.GetImageList(LVSIL_NORMAL);
	ASSERT(pImageList != NULL);

	for (int i = 0; i < m_listEmoticon.GetItemCount(); i++)
	{
		// 리스트로 부터 데이터를 가져온다.
		LVITEM item = {0};
		item.iItem = i;
		item.mask = LVIF_PARAM | LVIF_IMAGE;
		m_listEmoticon.GetItem(&item);
		Pattern* pPattern = (Pattern*) item.lParam;

		char name[32] = {0};
		strncpy(name, pPattern->name.c_str(), 32);
		name[31] = 0;

		BYTE* pIcon = new BYTE[32 * 32 * 4];
		PatternPacket packet;
		packet.samplingTime = pPattern->samplingTime;
		packet.nPointCount = pPattern->points.size();
		packet.dwPointsBytes = packet.nPointCount * sizeof(Pattern::Point);
		packet.dwIconBytes = GetBitmapBits((HBITMAP) pPattern->pIconBitmap->m_hObject, 32 * 32 * 4, pIcon);
		packet.dwPacketBytes = sizeof(packet) + packet.dwPointsBytes + packet.dwIconBytes;

		fwrite(name, 1, 32, fp);
		fwrite(&packet, 1, sizeof(packet), fp);
		fwrite(&pPattern->points[0], 1, sizeof(Pattern::Point) * packet.nPointCount, fp);
		fwrite(pIcon, 1, packet.dwIconBytes, fp);
	}
	fclose(fp);
}
