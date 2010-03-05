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


#endif /* NETWORKSIGNALS_H_ */
