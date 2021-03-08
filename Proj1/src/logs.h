#include <stdlib.h>
#include <time.h>

#include <stdio.h>
#define LOGFILE "LOG_FILENAME"

struct logInfo
{
    bool hasLogFile;
    FILE *logfile;
    clock_t startTime;
};

/**
 * Sets the log file
 */
void setLogfile(char **envp, struct logInfo *log);

/**
 * Sets the log start time
 */
void setLogStart(struct logInfo *log);

