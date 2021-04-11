#include "parsing.h"

int parseCMDArgs(char* argv[], Settings* settings) {
    char* timeFlag = argv[1];

    if (strcmp(timeFlag, "-t")) {     // returns non-zero if not equal
        fprintf(stderr, "Invalid 2nd argument! Should be '-t'\n");
        return 1;
    }

    int runningTime = atoi(argv[2]);
    if (runningTime < 1) {
        fprintf(stderr, "%s %s\n", "Invalid 3rd argument! The specified time",
            "should be an integer greater than 1");
        return 1;
    }

    char* fifoName = argv[3];
    if (!strcmp(fifoName, "")) {
        fprintf(stderr, "%s %s\n", "Invalid 4th argument! You need to",
             "specifiy a name for the public FIFO");
        return 1;
    }

    settings->execTime = runningTime, settings->fifoname = fifoName;
    return 0;
}
