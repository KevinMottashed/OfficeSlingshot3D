#if !defined(AFX_CHATSOCKET_H__F1A09BF2_8A8A_468B_A811_35F387F8F564__INCLUDED_)
#define AFX_CHATSOCKET_H__F1A09BF2_8A8A_468B_A811_35F387F8F564__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ChatSocket.h : header file
//

#include "ChatPacket.h"
#include "NetworkManager.h"

// Forward declaration (files include each other)
class NetworkManager;

// CChatSocket command target

/* WM_ON_ACCEPT
 * WPARAM : CChatSocket*	client socket
 * LPARAM : CChatSocket*	root parent or this
 */
#define WM_ON_ACCEPT	(WM_USER + 0x100)

/* WM_ON_CLOSE
 * WPARAM :
 * LPARAM : CChatSocket*	root parent or this
 */
#define WM_ON_CLOSE		(WM_USER + 0x101)

/* WM_ON_RECEIVE
 * WPARAM : vector<BYTE>*	recv queue
 * LPARAM : CChatSocket*	root parent or this
 */
#define WM_ON_RECEIVE	(WM_USER + 0x102)

/* WM_ON_PEER_DISCONNECT
 * WPARAM : CChatSocket*	client socket
 * LPARAM : CChatSocket*	root parent or this
 */
#define WM_ON_PEER_DISCONNECT	(WM_USER + 0x103)

/* WM_ON_NETWORK_ERROR
 * WPARAM : CChatSocket*	client socket
 * LPARAM : CChatSocket*	root parent or this
 */
#define WM_ON_NETWORK_ERROR	(WM_USER + 0x104)

/////////////////////////////////////////////////////////////////////////////
// CChatSocket command target

class CChatSocket : public CSocket
{
// Attributes
public:

// Operations
public:
	CChatSocket(NetworkManager* manager = NULL);
	virtual ~CChatSocket();

// Overrides
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChatSocket)
	public:
	virtual void OnAccept(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	virtual void OnSend(int nErrorCode);
	virtual int Send(const void* lpBuf, int nBufLen, int nFlags = 0);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CChatSocket)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	virtual int Send(CChatPacket& packet);
	virtual void Close();

	static CWnd* pWndMsgProc;

// Implementation
protected:
	int Send();
	CChatSocket* ParamPtr();

	bool m_bIsServer;
	CChatSocket* m_pParent;
	CChatSocket* m_pClient;

	vector<BYTE> m_sendQueue;
	vector<BYTE> m_recvQueue;

	NetworkManager* m_pNetworkManager;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHATSOCKET_H__F1A09BF2_8A8A_468B_A811_35F387F8F564__INCLUDED_)
