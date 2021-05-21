#ifndef SERVER_H_
#define SERVER_H_

#include "parsing.h"
#include "communication.h"

/**
 * Makes all the needed operations for the start of the program
 * @param argc Number of arguments
 * @param argv Command line arguments
 * @param settings Information stored for the rest of the program
*/
void init(int argc, char* argv[], Settings* settings);

/**
 * Makes all the needed operations before exiting the program
 * @param settings Information stored in the start of the program
 */
void exitProgram(Settings* settings);

#endif // SERVER_H_
