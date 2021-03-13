#ifndef LOGS_H_
#define LOGS_H_

#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <ctype.h>
#include <math.h>
#define LOGFILE "LOG_FILENAME"
#define START_TIME "XMOD_START_TIME"
#define FIRST_PID "XMOD_FIRST_PID"

typedef struct logInfo {
    bool hasLogFile;
    FILE *logfile;
    long startTime;
} logInfo;

/**
 * Sets the log file
 */
void setLogFile(char **envp, struct logInfo *log);

/**
 * Closes the log file
 */
void closeLogFile(struct logInfo *log);

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
void logChangePerms(struct logInfo *log, char*path, mode_t oldPerm,
                    mode_t newPerm);



#endif  // LOGS_H_
