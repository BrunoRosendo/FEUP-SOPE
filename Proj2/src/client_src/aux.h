#ifndef AUX_H_
#define AUX_H_

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define FIFO_PUBLIC_PERMS 0666

typedef struct Settings {
    unsigned execTime;
    char* fifoname;
} Settings;

/*
Makes all the needed operations for the start of the program
*/
void init(char* argv[], Settings* settings);

/**
 * Parses the command line arguments and puts the information in
 * the Settings struct
 * @return 0 on success, 1 otherwise
 */ 
int parseCMDArgs(char* argv[], Settings* settings);

/*
Creates a FIFO (if it doesn't exist) and waits for the server to synchronize
*/
void syncWithServer(Settings* settings);

#endif  // AUX_H_
