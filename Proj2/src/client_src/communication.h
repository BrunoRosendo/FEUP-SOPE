#ifndef COMM_H_
#define COMM_H_

#include "aux.h"

// Client request thread makes initial request
#define CLIENT_WANTS "IWANT"
// Main Server thread received the request
#define SERVER_RCVD "RECVD"
// Server producer thread has the result of the task
#define SERVER_PRODUCER_HAS_RESULT "TSKEX"
// Server consumer thread sent to the client the result of the task
#define SERVER_SENT_RESULT "TSKDN"
// Client request thread received the result of the task
#define CLIENT_RCVD "GOTRS"
// Server consumer thread rejects the task because the service has shut down
#define SERVER_REQUEST_2LATE "2LATE"
// Client request thread acknowledges request failure because server shut down
#define CLIENT_REQUEST_CLOSED "CLOSD"
// Client request thread gives up on request since client's running time is over
#define CLIENT_REQUEST_TIMEOUT "GAVUP"
// Server consumer thread can't answer since the private FIFO closed
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
