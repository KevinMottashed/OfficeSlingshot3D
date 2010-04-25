#ifndef NETWORK_SOCKET_H
#define NETWORK_SOCKET_H

#include "NetworkManager.h"

// STL & Windows
#include "StdAfx.h"

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

#endif

