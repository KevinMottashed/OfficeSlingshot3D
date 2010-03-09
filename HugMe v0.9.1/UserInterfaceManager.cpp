// UserInterfaceManager.cpp: implementation of the UserInterfaceManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "chat.h"
#include "UserInterfaceManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

UserInterfaceManager::UserInterfaceManager(Controller* pController) :
		m_pController(pController),
		m_pChatDlg(new CChatDlg())
{
}

UserInterfaceManager::~UserInterfaceManager()
{
	delete m_pChatDlg;
}

CDialog* UserInterfaceManager::getMainWindow()
{
	return m_pChatDlg;
}

void UserInterfaceManager::notifyNetworkConnectionEstablished()
{
	// TODO implement
}


