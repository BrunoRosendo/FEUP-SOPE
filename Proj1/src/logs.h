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
void setLogFile(char **envp, struct logInfo *log);

/**
 * Logs a line
 */
void logAction(struct logInfo *log, char*action, char *info);

/**
 * Sets the log start time
 */
void setLogStart(struct logInfo *log);

/** 
 * Logs process creation
 */
void logProcessCreation(struct logInfo *log, int argc, char *argv[]);

/**
 *  Logs process termination
 */
void logExit(struct logInfo *log, int exitStatus);

/**
 * Logs a received signal
 */ 
void logSignalReceived(struct logInfo *log, int signal);

/**
 * Logs a sent signal
 */
void logSignalSent(struct logInfo *log, int signal, int pid);

/**
 * Logs a change in permission
 */
void logChangePerms(struct logInfo *log, char*path, mode_t oldPerm, mode_t newPerm);

