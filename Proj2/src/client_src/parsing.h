#ifndef PARSING_H_
#define PARSING_H_

#include "aux.h"

/**
 * Parses the command line arguments and puts the information in the Settings struct
 * @param argv Command line arguments
 * @param settings return argument with the execution time of the program and the fifo name
 * @return 0 on success, 1 otherwise
 */ 
int parseCMDArgs(char* argv[], Settings* settings);

#endif
