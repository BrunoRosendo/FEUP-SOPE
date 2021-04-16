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
#include <sys/syscall.h>

/* CONSTANTS */

#define FIFO_PUBLIC_PERMS 0666

#define SYNC_WAIT_TIME 500
#define MAX_PATH_SIZE 500
#define MAX_REQUEST_SIZE 100

/* DATA STRUCTURES */

/**
 * Keeps the execution time and fifo info
 */
typedef struct Settings {
    time_t execTime; /** Program max execution time*/
    char fifoname[MAX_PATH_SIZE]; /** Public fifo's name*/
    int fd;  /** Public fifo's file descriptor */
} Settings;

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

#endif  // AUX_H_
