// Made by Sean P

#pragma once

#ifndef SP_IO_HPP
	#define SP_IO_HPP



#include <iostream> // cout, endl
#include <string> 	// string
#include <limits>	// numeric_limits

/////////////////////// Shell I/O ///////////////////////


/*SP_printIntro(std::string)
 * Prints the program name and author information.
 *
 *	@param programName	The name of the program.
 */
void SP_printIntro(std::string programName);


/*enterToContinue(std::string)
 * Prints a prompt message and waits for the user to press the Enter (Return) key.
 *
 *	@param prompt	The prompt message to print before the user presses Enter.
 */
void enterToContinue(std::string prompt);


/*enterToContinue(std::string, std::string)
 * Prints a prompt message, waits for the user to press Enter (Return) key, and
 *	prints a second message before continuing.
 *
 *	@param prompt	The prompt message to print before the user presses Enter.
 *	@param continueMessage	The message to print after the user presses Enter.
 */
void enterToContinue(std::string prompt, std::string continueMessage);


#endif // SP_IO_HPP