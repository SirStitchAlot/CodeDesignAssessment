/** InputHandler.h
 *
 * Handles user input.
 *
 * @author Hans de Ruiter
 *
 * @copyright (c) 2022 by Kea Sigma Delta Limited, all rights reserved
 *
 * See License.txt for license.
 */

#pragma once

class CommandListener;

/** Translates user input to commands that are passed on to a CommandListener.
 */
class InputHandler {
public:
	InputHandler();
	
	~InputHandler();
	
	/** Reads user input devices (keyboard, mouse, gamepad, etc.), and translates
	 * it to commands.
	 *
	 * @param listener the listener to send the commands to
	 */
	void handleInput(CommandListener &listener);

private:
	/** Player one's gamepad ID.
	 */
	int p1GamepadID;
};