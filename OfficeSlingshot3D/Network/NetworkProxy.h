/**
 * @file
 * Convinience header for including all public members of the network module.
 */

/**
 * @defgroup Network Network
 * The network module is responsible for sending and receiving data over the network.
 * 2 sockets are used to send and receive data. The control socket is used for all
 * data not relating to the game (chat messages, user names, ...). The data socket
 * is used for all game related data (projectiles, player positions, ...).
 */
// public members of the network module
#include "WinsockNetwork.h"
#include "Network.h"
#include "NetworkCodes.h"
#include "NetworkObserver.h"
#include "NetworkSubject.h"

// forward declarations
class WinsockNetwork;
class Network;
class NetworkObserver;
class NetworkSubject;

