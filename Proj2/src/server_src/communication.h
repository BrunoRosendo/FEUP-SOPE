#ifndef COMM_H_
#define COMM_H_

#include "aux.h"

/**
 * Creates a FIFO and waits for the client to synchronize
 * Updates the file descriptor in the settings
 * @param settings Keeps the fifo name that is to be created
*/
void syncWithClient(Settings* settings);

/**
 * Registers an operation to the stdout
 * @param rid request id
 * @param tskload task number
 * @param pid process id
 * @param tid thread id
 * @param tskres task result
 * @param oper type of operation
 */ 
void registerOperation(int rid, int tskload, int pid, pthread_t tid,
    int tskres, char* oper);

#endif