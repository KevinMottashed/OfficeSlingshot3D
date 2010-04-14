/*
 * NetworkSignals.h
 *
 *  Created on: Mar 4, 2010
 *      Author: kevin
 */

#ifndef NETWORKSIGNALS_H_
#define NETWORKSIGNALS_H_

/* WM_ON_ACCEPT
 * WPARAM : CChatSocket*	client socket
 * LPARAM : CChatSocket*	root parent or this
 */
#define WM_ON_ACCEPT	(WM_USER + 0x100)

/* WM_ON_CLOSE
 * WPARAM :
 * LPARAM : CChatSocket*	root parent or this
 */
#define WM_ON_CLOSE		(WM_USER + 0x101)

/* WM_ON_RECEIVE
 * WPARAM : vector<BYTE>*	recv queue
 * LPARAM : CChatSocket*	root parent or this
 */
#define WM_ON_RECEIVE	(WM_USER + 0x102)

/* WM_ON_CONNECT
 * WPARAM :
 * LPARAM :
 */
#define WM_ON_CONNECT	(WM_USER + 0x103)

/* WM_ON_DISCONNECT
 * WPARAM :
 * LPARAM :
 */
#define WM_ON_DISCONNECT	(WM_USER + 0x104)

/* WM_ON_NETWORK_ERROR
 * WPARAM :
 * LPARAM :
 */
#define WM_ON_NETWORK_ERROR	(WM_USER + 0x105)

/* WM_ON_START_GAME
 * WPARAM :
 * LPARAM :
 */
#define WM_ON_START_GAME	(WM_USER + 0x106)

/* WM_ON_EXIT_GAME
 * WPARAM :
 * LPARAM :
 */
#define WM_ON_EXIT_GAME (WM_USER + 0x107)

/* WM_ON_PAUSE_GAME
 * WPARAM :
 * LPARAM :
 */
#define WM_ON_PAUSE_GAME (WM_USER + 0x108)

/* WM_ON_SEND_CHAT_MESSAGE
 * WPARAM : remote Username
 * LPARAM : message
 */
#define WM_ON_NEW_CHAT_MESSAGE (WM_USER + 0x109)

/* WM_ON_NEW_LOCAL_FRAME
 * WPARAM : video frame
 */
#define WM_ON_NEW_LOCAL_FRAME (WM_USER + 0x110)

/* WM_ON_NEW_REMOTE_FRAME
 * WPARAM : video frame
 */
#define WM_ON_NEW_REMOTE_FRAME (WM_USER + 0x111)


#endif /* NETWORKSIGNALS_H_ */
