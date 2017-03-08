// Made by Sean P

#pragma once

#ifndef SP_IO_HPP
	#define SP_IO_HPP



#include <iostream> // cout, endl
#include <limits>	// numeric_limits

/////////////////////// Shell I/O ///////////////////////


/*sp_print_intro(const char*)
 * Prints the program name and author information.
 *
 *	@param programName	The name of the program.
 */
void sp_print_intro(const char* programName);


/*enter_to_continue(const char*)
 * Prints a prompt message and waits for the user to press the Enter (Return) key.
 *
 *	@param prompt	The prompt message to print before the user presses Enter.
 */
void enter_to_continue(const char* prompt);


/*enter_to_continue(const char*, const char*)
 * Prints a prompt message, waits for the user to press Enter (Return) key, and
 *	prints a second message before continuing.
 *
 *	@param prompt	The prompt message to print before the user presses Enter.
 *	@param continueMessage	The message to print after the user presses Enter.
 */
void enter_to_continue(const char* prompt, const char* continueMessage);


#endif // SP_IO_HPP