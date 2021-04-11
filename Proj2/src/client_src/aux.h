#ifndef AUX_H_
#define AUX_H_

/* INCLUDES */

// C Libs
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>

// System libs
#include <sys/types.h>
#include <sys/stat.h>

/* CONSTANTS */

#define FIFO_PUBLIC_PERMS 0666

#define SYNC_WAIT_TIME 500
#define MAX_PATH_SIZE 500
#define MAX_REQUEST_SIZE 30

/* DATA STRUCTURES */

typedef struct Settings {
    time_t execTime;
    char fifoname[MAX_PATH_SIZE];
    int fd;  // File (fifo) descriptor
} Settings;

typedef struct Message {
    int rid;        // request id
    pid_t pid;      // process id
    pthread_t tid;  // thread id
    int tskload;   // task load
    int tskres;    // task result
} Message;

#endif  // AUX_H_
