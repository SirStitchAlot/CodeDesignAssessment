/** CommandListener.h
 *
 * @author Hans de Ruiter
 *
 * @copyright (c) 2022 by Kea Sigma Delta Limited, all rights reserved
 *
 * See License.txt for license.
 */

#pragma once

/** Base class for objects that need to listen to user (or AI) commands.
 */
class CommandListener {
public:
	CommandListener();
	
	virtual ~CommandListener();
	
	// Child classes, override the methods below that you need to respond
	// to user/AI commands
	
	virtual void goUp();
	
	virtual void goLeft();
	
	virtual void goRight();
	
	virtual void goDown();
	
	/** Called when none of the other go*() are called. 
	 */
	virtual void goNowhere();
	
	/** Select, push, pull, punch, fire, etc.
	 */
	virtual void doInteractWith();
	
	/** The ESCape key was pushed, the user wants to quit.
	 */
	virtual void doExit();
	
	/** Handle miscellaneous key presses.
	 */
	virtual void handleKeyPress(int key);
};
