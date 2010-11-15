#ifndef NETWORK_CODES_H
#define NETWORK_CODES_H

#include "stdafx.h"

/**
 * @ingroup Network
 * @b public
 * All the possible return codes from network related functions.
 */
enum rc_network
{
	SUCCESS, /**< Success */
	ERROR_ALREADY_LISTENING, /**< Error could not perform operation, already listening. */
	ERROR_CREATE_CONTROL_SOCKET, /**< Error could not create the control socket. */
	ERROR_CREATE_DATA_SOCKET, /**< Error could not create the data socket. */
	ERROR_LISTEN_CONTROL_SOCKET, /**< Error could not listen on the control socket. */
	ERROR_LISTEN_DATA_SOCKET, /**< Error could not listen on the data socket. */
	ERROR_ALREADY_CONNECTED, /**< Error could not perform operation, already connected. */
	ERROR_CONNECT_CONTROL_SOCKET, /**< Error could not connect the control socket. */
	ERROR_CONNECT_DATA_SOCKET, /**< Error could not connect the data socket. */
	ERROR_SOCKET_ERROR, /**< A socket error has occured. */
	ERROR_NO_CONNECTION, /**< Error could not perform operation, not connected to a peer. */
	ERROR_USER_NAME_NOT_RECEIVED, /**< Error the peer's username was not received while establishing the connection. */
};

/**
 * Convert the error code to a string.
 * @param[in] code The enum to convert.
 * @return The string representation of the enum.
 */
static std::string lookup(rc_network code)
{
	switch(code)
	{
	case SUCCESS:
		return "SUCCESS";
	case ERROR_ALREADY_LISTENING:
		return "Error, already listening";
	case ERROR_CREATE_CONTROL_SOCKET:
		return "Error, failed to create control socket";
	case ERROR_CREATE_DATA_SOCKET:
		return "Error, failed to create data socket";
	case ERROR_LISTEN_CONTROL_SOCKET:
		return "Error, failed to listen on control socket";
	case ERROR_LISTEN_DATA_SOCKET:
		return "Error, failed to listen on data socket";
	case ERROR_ALREADY_CONNECTED:
		return "Error, already connected";
	case ERROR_CONNECT_CONTROL_SOCKET:
		return "Error, could not connect the control socket";
	case ERROR_CONNECT_DATA_SOCKET:
		return "Error, could not connect the data socket";
	case ERROR_SOCKET_ERROR:
		return "Error, a socket error has occurred";
	case ERROR_NO_CONNECTION:
		return "Error, not connected";
	default:
		return "unknown error code";
	}
}

#endif

