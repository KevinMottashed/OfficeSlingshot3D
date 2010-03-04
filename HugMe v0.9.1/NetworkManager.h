// NetworkManager.h: interface for the NetworkManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NETWORKMANAGER_H__3D85BBC3_3F80_477F_ABAB_1DAE8326532A__INCLUDED_)
#define AFX_NETWORKMANAGER_H__3D85BBC3_3F80_477F_ABAB_1DAE8326532A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//STL
#include <string>

#include "Controller.h"
#include "ChatSocket.h"
#include "NetworkCodes.h"

// Forward declarations (files include each other)
class Controller;
class CChatSocket;

class NetworkManager  
{
public:
	NetworkManager(Controller* pController);
	virtual ~NetworkManager();

	// start listening for connections
	rc_network startListening();

	// notify the controller that a network connection has been accepted
	void notifyAccept();

private:
	// the network port for our application
	static const int chat_port;

	CChatSocket* m_pChatSocket;
	Controller* m_pController;
};

#endif // !defined(AFX_NETWORKMANAGER_H__3D85BBC3_3F80_477F_ABAB_1DAE8326532A__INCLUDED_)
