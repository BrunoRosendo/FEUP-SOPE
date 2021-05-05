#ifndef _COMMON_H
#define _COMMON_H

/* INCLUDES */

#include <sys/types.h>

/* CONSTANTS */

#define FIFO_PUBLIC_PERMS 0666

#define SYNC_WAIT_TIME 500
#define MAX_PATH_SIZE 500
#define MAX_REQUEST_SIZE 100

// Time variables
#define MAX_TIME_WAITING_FIFO 30
#define TIME_BETWEEN_ATTEMPTS_FIFO 100

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


/* DATA STRUCTURES */

/**
 * Message that is transmited through a fifo
 */
typedef struct Message {
    int rid; /** Request id */
    pid_t pid; /** Process id */
    pthread_t tid; /** Thread id */
    int tskload; /** Task load */
    int tskres; /** Task result */
} Message;

#endif // _COMMON_H
