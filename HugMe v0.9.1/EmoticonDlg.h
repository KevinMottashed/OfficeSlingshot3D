#if !defined(AFX_EMOTICONDLG_H__00606073_5726_4288_BCBF_D7685D41FA7E__INCLUDED_)
#define AFX_EMOTICONDLG_H__00606073_5726_4288_BCBF_D7685D41FA7E__INCLUDED_


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EmoticonDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEmoticonDlg dialog

class CEmoticonDlg : public CDialog
{
// Construction
public:
	CEmoticonDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CEmoticonDlg)
	enum { IDD = IDD_EMOTICON_DIALOG };
	CListCtrl	m_listEmoticon;
	//}}AFX_DATA

	void LoadEmoticons();
	void SaveEmoticons();
	BOOL m_bHideWhenInactive;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEmoticonDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEmoticonDlg)
	afx_msg void OnActivate( UINT, CWnd*, BOOL );
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnButtonNew();
	afx_msg void OnButtonClose();
	afx_msg void OnButtonEdit();
	afx_msg void OnButtonSend();
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonDelete();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EMOTICONDLG_H__00606073_5726_4288_BCBF_D7685D41FA7E__INCLUDED_)
