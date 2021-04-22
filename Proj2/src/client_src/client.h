#ifndef CLIENT_H_
#define CLIENT_H_

#include "parsing.h"
#include "communication.h"

/**
 * Makes all the needed operations for the start of the program
 * @param argv Command line arguments
 * @param settings Information kept for the production of the requests
 * @return 0 upon success, 1 otherwise 
*/
int init(char* argv[], Settings* settings);

#endif
