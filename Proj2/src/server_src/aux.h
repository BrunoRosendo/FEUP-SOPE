#ifndef AUX_H_
#define AUX_H_

/* INCLUDES */

// C Libs
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <fcntl.h>
#include <unistd.h>
#include <semaphore.h>

// System libs
#include <sys/types.h>
#include <sys/stat.h>

#include "../common.h"

/* CONSTANTS */
#define MAX_BUFFER_SIZE 500

/* DATA STRUCTURES */

/**
 * Keeps the execution time, fifo info and buffer size
 */
typedef struct Settings {
    time_t execTime; /** Program max execution time */
    char fifoname[MAX_PATH_SIZE]; /** Public fifo's name */
    int bufferSize;  /** Size of the buffer used to store results */
} Settings;

#endif  // AUX_H_
