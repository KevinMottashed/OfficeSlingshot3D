#include "NetworkSocket.h"

NetworkSocket::NetworkSocket(WinsockNetwork* network) :
	network(network), m_bIsServer(false), m_pClient(NULL)
{
}

NetworkSocket::~NetworkSocket()
{
	delete m_pClient;
}

SOCKET NetworkSocket::Detach()
{
	if (m_bIsServer)
	{
		CSocket::Close();
		return m_pClient->Detach();
	}
	return CSocket::Detach();
}

void NetworkSocket::OnAccept(int nErrorCode) 
{
	m_bIsServer = true;
	m_pClient = new NetworkSocket(network);
	Accept(*m_pClient);

	network->notifyAccept(this);

	CSocket::OnAccept(nErrorCode);
}

bool NetworkSocket::isServer() const
{
	return m_bIsServer;
}

