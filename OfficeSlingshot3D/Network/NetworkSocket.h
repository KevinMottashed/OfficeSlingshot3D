#ifndef NETWORK_SOCKET_H
#define NETWORK_SOCKET_H

// STL & Windows
#include "StdAfx.h"

#include "WinsockNetwork.h"

// Forward declaration (files include each other)
class WinsockNetwork;

/**
 * @ingroup Network
 * @b public
 * A network socket used to transmit/receive data.
 */
class NetworkSocket : public CSocket
{
public:
	/**
	 * Constructor.
	 * @param[in] network The network manager that will be notified when the socket has accepted a connection.
	 */
	NetworkSocket(WinsockNetwork* network);

	/** Destructor. */
	virtual ~NetworkSocket();

	/**
	 * Detach the socket.
	 * This will detach the thread that is managing this socket so that it can be managed manually.
	 * @return The socket handle that was detached.
	 */
	virtual SOCKET Detach();

	/**
	 * Find out if the socket is in listen mode.
	 * @return true if the socket is in listen mode.
	 */
	virtual bool isServer() const;

	/**
	 * Callback function for when the socket has accepted a connection.
	 * @param[in] nErrorCode The error code that happened when the connection was accepted.
	 */
	virtual void OnAccept(int nErrorCode);

private:
	/** The network manager to be notified when the socket has accepted a connection. */
	WinsockNetwork* network;
	bool m_bIsServer; /**< True if the socket is acting as a server (listen mode) */
	NetworkSocket* m_pClient; /**< Client socket, will be used to accept the connection. */
};

#endif

