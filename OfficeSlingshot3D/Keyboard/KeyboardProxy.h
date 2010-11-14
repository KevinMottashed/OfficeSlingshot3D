/**
 * @file
 * Convinience header for including all public members of the keyboard module.
 */

/**
 * @defgroup Keyboard Keyboard
 * The keyboard module is used to abstract out the keyboard.
 * In MFC apps only UI classes can read inputs from the keyboard.
 * This module lets listeners be notified when a key on the keyboard
 * is pressed without creating all kinds of dependencies to the UI.
 */
#include "Keyboard.h"
#include "KeyboardListener.h"

class Keyboard;
class KeyboardListener;