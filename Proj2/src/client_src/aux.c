#include "aux.h"

int parseCMDArgs(char* argv[], Settings* settings) {
    char* timeFlag = argv[1];
    
    if(strcmp(timeFlag, "-t")){     // returns non-zero if not equal
        fprintf(stderr, "Invalid 2nd argument! Should be '-t'\n");
        return 1;
    }

    int runningTime = atoi(argv[2]);
    if(runningTime < 1){
        fprintf(stderr, "Invalid 3rd argument! The specified time should be an integer greater than 1\n");
        return 1;
    }

    char* fifoName = argv[3];
    if(!strcmp(fifoName, "")){
        fprintf(stderr, "Invalid 4th argument! You need to specifiy a name for the public FIFO\n");
        return 1;
    }

    settings->execTime = runningTime, settings->fifoname = fifoName;
    return 0;
}

void syncWithServer(Settings* settings) {
    // There's an error if the server already created the FIFO
    mkfifo(settings->fifoname, FIFO_PUBLIC_PERMS);

    // sync the client with the server
    while (open(settings->fifoname, O_RDWR) < 0) {}
}

void init(char* argv[], Settings* settings) {
    if(parseCMDArgs(argv, settings))
        return;

    syncWithServer(settings);
}
