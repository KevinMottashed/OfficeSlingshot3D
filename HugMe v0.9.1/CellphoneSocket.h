#if !defined(AFX_CELLPHONESOCKET_H__F3878180_84E5_4572_96C9_4D9E1E6C7047__INCLUDED_)
#define AFX_CELLPHONESOCKET_H__F3878180_84E5_4572_96C9_4D9E1E6C7047__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CellphoneSocket.h : header file
//

#include "HumanModel.h"

#define CELLPHONE_PORT 7500

// CChatSocket command target

/* WM_ON_ACCEPT
 * WPARAM : CChatSocket*	client socket
 * LPARAM : CChatSocket*	root parent or this
 */
#define WM_ON_ACCEPT_CELL	(WM_USER + 0x200)

/* WM_ON_CLOSE
 * WPARAM :
 * LPARAM : CChatSocket*	root parent or this
 */
#define WM_ON_CLOSE_CELL		(WM_USER + 0x201)

/* WM_ON_RECEIVE
 * WPARAM : vector<BYTE>*	recv queue
 * LPARAM : CChatSocket*	root parent or this
 */
#define WM_ON_RECEIVE_CELL	(WM_USER + 0x202)


typedef struct tagCellphoneContact {
	bool m_bIsContacted; // 0 or 1
	int m_part; // 0 or 1 or 2
	float m_x;
	float m_y;
} CellphoneContact;

/////////////////////////////////////////////////////////////////////////////
// CellphoneSocket command target

class CellphoneSocket : public CSocket
{
// Attributes
public:
	static CWnd* pWndMsgProc;
	CellphoneSocket* m_pParent;
	CellphoneSocket* m_pClient;

	CellphoneContact m_cellphoneContact;

// Operations
public:
	CellphoneSocket();
	virtual ~CellphoneSocket();

// Overrides
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CellphoneSocket)
	public:
	virtual void OnAccept(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	//}}AFX_VIRTUAL

// Implementation
	// Generated message map functions
	//{{AFX_MSG(CellphoneSocket)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CELLPHONESOCKET_H__F3878180_84E5_4572_96C9_4D9E1E6C7047__INCLUDED_)
