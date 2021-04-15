#ifndef COMM_H_
#define COMM_H_

#include "aux.h"

#define CLIENT_WANTS "IWANT"
#define SERVER_RCVD "RECVD"
#define SERVER_PRODUCER_HAS_RESULT "TSKEX"
#define SERVER_SENT_RESULT "TSKDN"
#define CLIENT_RCVD "GOTRS"
#define SERVER_REQUEST_2LATE "2LATE"
#define CLIENT_REQUEST_CLOSED "CLOSD"
#define CLIENT_REQUEST_TIMEOUT "GAVUP"
#define SERVER_PRIVATE_FIFO_CLOSED "FAILD"

/*
Creates a FIFO (if it doesn't exist) and waits for the server to synchronize
*/
void syncWithServer(Settings* settings);

/*
Main loop of the program. Generates threads which make requests to the server
Terminates the program when the execution time is due
*/
void generateRequests(Settings* settings);

/*
Function used by a newborn thread
@param arg Public fifo name passed by the main loop
*/
void *makeRequest(void* arg);

/**
 * Registers an operation to the stdout
 * @param rid 
 * @param tskload
 * @param pid
 * @param tid
 * @param tskres  
 * @param oper type of operation
 */ 
void registerOperation(int rid, int tskload, int pid, pthread_t tid,
    int tskres, char* oper);

#endif
