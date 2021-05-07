#include "parsing.h"

int parseCMDArgs(int argc, char* argv[], Settings* settings) {
    int i = 1;
    char* timeFlag = argv[i++];

    if (strcmp(timeFlag, "-t")) {     // returns non-zero if not equal
        fprintf(stderr, "Invalid 2nd argument! Should be '-t'\n");
        return 1;
    }

    time_t runningTime = atoi(argv[i++]);
    if (runningTime < 1) {
        fprintf(stderr, "%s %s\n", "Invalid 3rd argument! The specified time",
            "should be an integer greater than 1");
        return 1;
    }

    if (argc == 6) {
        char* bufferFlag = argv[i++];

        if (strcmp(bufferFlag, "-l")) {
            fprintf(stderr, "Invalid 4th argument! Should be '-l'\n");
            return 1;
        }

        int bufferSize = atoi(argv[i++]);
        if (bufferSize < 1) {
            fprintf(stderr, "%s %s\n", "Invalid 5th argument! The buffer size",
                "should be an integer greater than 1");
            return 1;
        }
        settings->bufferSize = bufferSize;
    } else {
        settings->bufferSize = MAX_BUFFER_SIZE;
    }

    char* fifoName = argv[i];
    if (!strcmp(fifoName, "")) {
        fprintf(stderr, "%s %s\n", "Invalid 4th argument! You need to",
             "specifiy a name for the public FIFO");
        return 1;
    }

    settings->execTime = runningTime;
    snprintf(settings->fifoname, sizeof(settings->fifoname), "%s", fifoName);
    return 0;
}
