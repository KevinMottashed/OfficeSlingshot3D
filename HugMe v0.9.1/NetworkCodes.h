#ifndef _NETWORK_CODES_H__
#define _NETWORK_CODES_H__

// All the possible return codes from network related functions
enum rc_network
{
	SUCCESS,
	ERROR_ALREADY_LISTENING,
	ERROR_CREATE_SOCKET,
	ERROR_LISTEN_SOCKET,
	ERROR_ALREADY_CONNECTED,
	ERROR_CONNECT,
	ERROR_MESSAGE_QUEUE_OVERFLOW
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
	case ERROR_CREATE_SOCKET:
		return "Error, failed to create socket";
	case ERROR_LISTEN_SOCKET:
		return "Error, failed to listen on socket";
	case ERROR_ALREADY_CONNECTED:
		return "Error, already connected";
	case ERROR_CONNECT:
		return "Error, could not connect";
	case ERROR_MESSAGE_QUEUE_OVERFLOW:
		return "Error, too many message in the queue";
	default:
		return "unknown error code";
	}
}

#endif
