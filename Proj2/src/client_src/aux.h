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

#endif  // AUX_H_
