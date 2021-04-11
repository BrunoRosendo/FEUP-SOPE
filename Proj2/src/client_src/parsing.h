#ifndef PARSING_H_
#define PARSING_H_

#include "aux.h"

/**
 * Parses the command line arguments and puts the information in
 * the Settings struct
 * @return 0 on success, 1 otherwise
 */ 
int parseCMDArgs(char* argv[], Settings* settings);

#endif
