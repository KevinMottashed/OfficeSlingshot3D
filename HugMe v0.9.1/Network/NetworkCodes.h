#ifndef _NETWORK_CODES_H__
#define _NETWORK_CODES_H__

#include "Stdafx.h"

// All the possible return codes from network related functions
enum rc_network
{
	SUCCESS,
	ERROR_ALREADY_LISTENING,
	ERROR_CREATE_CONTROL_SOCKET,
	ERROR_CREATE_DATA_SOCKET,
	ERROR_LISTEN_CONTROL_SOCKET,
	ERROR_LISTEN_DATA_SOCKET,
	ERROR_ALREADY_CONNECTED,
	ERROR_CONNECT_CONTROL_SOCKET,
	ERROR_CONNECT_DATA_SOCKET,
	ERROR_MESSAGE_QUEUE_OVERFLOW,
	ERROR_SOCKET_ERROR,
	ERROR_UNKNOWN_CONTROL_MESSAGE,
	ERROR_UNKNOWN_DATA_MESSAGE,
	ERROR_NO_CONNECTION,
	ERROR_USER_NAME_NOT_RECEIVED
};

// Get the error message for a return code
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
	case ERROR_MESSAGE_QUEUE_OVERFLOW:
		return "Error, too many message in the queue";
	case ERROR_SOCKET_ERROR:
		return "Error, a socket error has occurred";
	case ERROR_UNKNOWN_CONTROL_MESSAGE:
		return "Error, unknown control message received";
	case ERROR_UNKNOWN_DATA_MESSAGE:
		return "Error, unknown data message received";
	case ERROR_NO_CONNECTION:
		return "Error, not connected";
	default:
		return "unknown error code";
	}
}

#endif
