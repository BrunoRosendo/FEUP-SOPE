#ifndef COMM_H_
#define COMM_H_

#include "aux.h"
#include <unistd.h>

static 

/*
Creates a FIFO (if it doesn't exist) and waits for the server to synchronize
*/
void syncWithServer(Settings* settings);

/*
Main loop of the program. Generates threads which make requests to the server
Terminates the program when the execution time is due
*/
void generateRequests(Settings* settings);

#endif