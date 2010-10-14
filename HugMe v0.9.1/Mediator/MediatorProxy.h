/**
 * @file
 * Convinience header for including all public members of the mediator module.
 */

/**
 * @defgroup Mediator Mediator
 * The mediator module is responsible for mediating interaction between the devices, the peer and the game.
 * The mediator is responsible for knowing how to respond to game events. For example, if the local player gets hit
 * the mediator will notify the peer, vibrate the jacket and play an audio cue. The mediator is also responsible
 * for synchronizing both clients.
 */
#include "Mediator.h"
#include "MediatorObserver.h"
#include "MediatorSubject.h"

// public members
class Mediator;
class MediatorObserver;
class MediatorSubject;

