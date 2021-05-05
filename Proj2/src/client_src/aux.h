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
#include <sys/stat.h>
#include <sys/syscall.h>

#include "../common.h"

/* DATA STRUCTURES */

/**
 * Keeps the execution time and fifo info
 */
typedef struct Settings {
    time_t execTime; /** Program max execution time*/
    char fifoname[MAX_PATH_SIZE]; /** Public fifo's name*/
    int fd;  /** Public fifo's file descriptor */
} Settings;

#endif  // AUX_H_
