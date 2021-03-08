#include <stdbool.h>
#include <string.h>
#include "logs.h"

void setLogfile(char **envp, struct logInfo *log)
{
    for (char **env = envp; *env != 0; env++)
    {
        char *thisEnv = *env;
        if (strstr(thisEnv, LOGFILE) != 0)
        {
            const char delim[2] = "=";
            strtok(thisEnv, delim);
            char *filename = strtok(NULL, delim);
            log->logfile = fopen(filename, "w");
            log->hasLogFile = true;
        }
    }
    log->hasLogFile= false;
}

void setLogStart(struct logInfo *log)
{
    log->startTime = clock();
}