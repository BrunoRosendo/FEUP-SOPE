#ifndef COMM_H_
#define COMM_H_

#include "aux.h"

/**
 * Creates a FIFO (if it doesn't exist) and waits for the server to synchronize
 * @param settings Keeps the fifo name that is to be created
*/
void syncWithServer(Settings* settings);

/**
 * Main loop of the program. Generates threads which make requests to the server
 * Terminates the program when the execution time is due
 * @param settings Information kept for the production of the requests
*/
void generateRequests(Settings* settings);

/**
 * Function used by a newborn thread
 * @param arg Public fifo name passed by the main loop
*/
void *makeRequest(void* arg);

#endif
