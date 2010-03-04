// ChatSocket.cpp : implementation file
//

#include "stdafx.h"
#include "Chat.h"
#include "ChatSocket.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//CWnd* CChatSocket::pWndMsgProc = NULL;

/////////////////////////////////////////////////////////////////////////////
// CChatSocket

CChatSocket::CChatSocket(NetworkManager* manager)
	: m_bIsServer(false), m_pParent(NULL), m_pClient(NULL), m_pNetworkManager(manager)
{
}

CChatSocket::~CChatSocket()
{
	if (m_bIsServer)
	{
		m_pClient->Close();
		delete m_pClient;
		m_pClient = NULL;
		m_bIsServer = false;
	}
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CChatSocket, CSocket)
	//{{AFX_MSG_MAP(CChatSocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CChatSocket member functions

void CChatSocket::OnAccept(int nErrorCode) 
{
	m_bIsServer = true;
	m_pClient = new CChatSocket();
	m_pClient->m_pParent = this;
	Accept(*m_pClient);

	//if (pWndMsgProc)
	//{
	//	pWndMsgProc->SendMessage(WM_ON_ACCEPT, (WPARAM) (LPCSTR) m_pClient, (LPARAM) ParamPtr());
	//}
	if (m_pNetworkManager)
	{
		m_pNetworkManager->notifyAccept();
	}

	CSocket::OnAccept(nErrorCode);
}

void CChatSocket::OnClose(int nErrorCode) 
{
	//if (pWndMsgProc)
	//{
	//	pWndMsgProc->SendMessage(WM_ON_CLOSE, 0, (LPARAM) ParamPtr());
	//}
	CSocket::OnClose(nErrorCode);
}

void CChatSocket::OnReceive(int nErrorCode) 
{
	char buff[4097];
	int ret = Receive(buff, 4096);

	switch (ret) {
	case 0:
		Close();
		break;

	case SOCKET_ERROR:
		if (GetLastError() != WSAEWOULDBLOCK)
		{
			Close();
		}
		break;

	default:
		for (int i = 0; i < ret; i++) {
			m_recvQueue.push_back(buff[i]);
		}
//		m_recvQueue.insert(m_recvQueue.end(), &buff[0], &buff[ret]);
	}

	//if (pWndMsgProc) {
	//	pWndMsgProc->SendMessage(WM_ON_RECEIVE, (WPARAM) &m_recvQueue, (LPARAM) ParamPtr());
	//}
	CSocket::OnReceive(nErrorCode);
}

void CChatSocket::OnSend(int nErrorCode) 
{
	if (m_sendQueue.size() > 0)
	{
		Send();
	}
	
	CSocket::OnSend(nErrorCode);
}

int CChatSocket::Send(const void* lpBuf, int nBufLen, int nFlags) 
{
	if (m_bIsServer)
	{
		return m_pClient->Send(lpBuf, nBufLen, nFlags);
	}

	int i;
	for (i = 0; i < 4; i++) {
		m_sendQueue.push_back(((char*) &nBufLen)[i]);
	}
	for (i = 0; i < nBufLen; i++) {
		m_sendQueue.push_back(((char*) lpBuf)[i]);
	}
//	m_sendQueue.insert(m_sendQueue.end(), (char*) &nBufLen, (char*) (&nBufLen + 1));
//	m_sendQueue.insert(m_sendQueue.end(), (char*) lpBuf, (char*) lpBuf + nBufLen);

	return Send();
}
int CChatSocket::Send(CChatPacket& packet)
{
	if (m_bIsServer)
	{
		return m_pClient->Send(packet);
	}

	DWORD packetSize = packet.getPacketSize();
	CChatPacket::PacketType packetType = packet.getType();
	DWORD totalSize = sizeof(packetSize) + sizeof(packetType) + packetSize;

	int i;
	for (i = 0; i < 4; i++) {
		m_sendQueue.push_back(((char*) &totalSize)[i]);
	}
	for (i = 0; i < 4; i++) {
		m_sendQueue.push_back(((char*) &packetType)[i]);
	}
//	m_sendQueue.insert(m_sendQueue.end(), (char*) &totalSize, (char*) (&totalSize + 1));
//	m_sendQueue.insert(m_sendQueue.end(), (char*) &packetType, (char*) (&packetType + 1));
	if (packetSize > 0) {
		BYTE* pData = packet.getPacketPtr();
		m_sendQueue.reserve(totalSize + m_sendQueue.size());
		for (i = 0; i < (int)packetSize; i++) {
			m_sendQueue.push_back(pData[i]);
		}
//		m_sendQueue.insert(m_sendQueue.end(), (char*) packet.getPacketPtr(), (char*) (packet.getPacketPtr() + packetSize));
	}

	return Send();
}

int CChatSocket::Send()
{
	int sendLength = (int) m_sendQueue.size();

	// 한번에 보낼 데이터 크기를 10k로 한정
	if (sendLength > 10240) {
		sendLength = 10240;
	}

	int ret = CSocket::Send((void*) &m_sendQueue.front(), sendLength, 0);

	switch (ret) {
		case 0:
			break;

		case SOCKET_ERROR:
			if (GetLastError() != WSAEWOULDBLOCK)
			{
				Close();
			}
			break;

		default:
			m_sendQueue.erase(m_sendQueue.begin(), m_sendQueue.begin() + ret);
	}
	return ret;
}

void CChatSocket::Close()
{
	if (m_bIsServer) {
		if (m_pClient) {
			m_pClient->Close();
			delete m_pClient;
			m_pClient = NULL;
		}
		m_bIsServer = false;
	}

	CSocket::Close();
}

CChatSocket* CChatSocket::ParamPtr()
{
	return (m_pParent) ? m_pParent->ParamPtr() : this;
}
