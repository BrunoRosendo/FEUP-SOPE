#ifndef AUX_H_
#define AUX_H_

/* INCLUDES */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

/* CONSTANTS */

#define FIFO_PUBLIC_PERMS 0666

#define SYNC_WAIT_TIME 500

/* DATA STRUCTURES */

typedef struct Settings {
    unsigned execTime;
    char* fifoname;
} Settings;

typedef struct Message {
    int rid;        // request id
    pid_t pid;      // process id
    pthread_t tid;  // thread id
    int tskload;   // task load
    int tskres;    // task result
} Message;

#endif  // AUX_H_
