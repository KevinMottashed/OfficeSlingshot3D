#if !defined(AFX_NETWORKSOCKET_H__75C413B1_CF23_4933_AAC8_800170373195__INCLUDED_)
#define AFX_NETWORKSOCKET_H__75C413B1_CF23_4933_AAC8_800170373195__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NetworkSocket.h : header file
//

#include "NetworkManager.h"

// Forward declaration (files include each other)
class NetworkManager;

/////////////////////////////////////////////////////////////////////////////
// NetworkSocket command target

class NetworkSocket : public CSocket
{
// Attributes
public:

// Operations
public:
	NetworkSocket(NetworkManager* pManager);
	virtual ~NetworkSocket();

	virtual SOCKET Detach();
	virtual bool isServer() const;


// Overrides
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(NetworkSocket)
	public:
	virtual void OnAccept(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(NetworkSocket)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
private:
	NetworkManager* m_pManager;
	bool m_bIsServer;
	NetworkSocket* m_pClient;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NETWORKSOCKET_H__75C413B1_CF23_4933_AAC8_800170373195__INCLUDED_)
