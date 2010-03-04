#ifndef _NETWORK_CODES_H__
#define _NETWORK_CODES_H__

// All the possible return codes from network related functions
enum rc_network
{
	SUCCESS,
	ERROR_ALREADY_LISTENING,
	ERROR_CREATE_SOCKET,
	ERROR_LISTEN_SOCKET
};

// Get the error message for a return code
static std::string lookup(rc_network code)
{
	switch(code)
	{
	case SUCCESS:
		return "SUCCESS";
	case ERROR_ALREADY_LISTENING:
		return "Error, Already Listening";
	case ERROR_CREATE_SOCKET:
		return "Error, failed to create socket";
	case ERROR_LISTEN_SOCKET:
		return "Error, failed to listen on socket";
	default:
		return "unknown error code";
	}
}

#endif
