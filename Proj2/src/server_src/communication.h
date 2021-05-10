#ifndef COMM_H_
#define COMM_H_

#include "aux.h"

void setupForLoop();

void exitLoop(int lastThread);

/**
 * Waits for all the created threads to finish their tasks. Also frees the threads array
 * @param numThreads Index of the last created thread
 */
void waitForAllThreads(int lastThread);

/**
 * Main loop of the program. Does everything important
 */
void listenAndRespond(Settings* settings);

/**
 * New-born thread function, to process requests by calling the library
 */
void *processRequest(void* arg);

void dispatchResults();

void getNewRequest(int* i);

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