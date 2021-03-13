#include "logs.h"

void setLogFile(char **envp, struct logInfo *log) {
    for (char **env = envp; *env != 0; env++)     {
        char *thisEnv = *env;
        if (strstr(thisEnv, LOGFILE) != 0) {
            const char delim[2] = "=";
            strtok(thisEnv, delim);
            char *filename = strtok(NULL, delim);
            log->logfile = fopen(filename, "w");
            log->hasLogFile = true;
            return;
        }
    }
    log->hasLogFile = false;
}

void closeLogFile(struct logInfo *log) {
    if (log->hasLogFile) {
        fclose(log->logfile);
    }
}

void setLogStart(struct logInfo *log) {
    if(getenv(FIRST_PID)){      // if the env variable already exists
        log->startTime = atol(getenv(START_TIME));
    } else {
        struct timespec time;
        clock_gettime(CLOCK_REALTIME, &time);
        log->startTime = time.tv_sec*1000 + time.tv_nsec/(pow(10, 6));

        char pidString[10];
        char startTimeString[50];
        snprintf(pidString, sizeof(pidString), "%d", getpid());
        snprintf(startTimeString, sizeof(startTimeString) , "%ld", log->startTime);

        int stat = setenv(START_TIME, startTimeString, 1);
        if(stat == -1){
            fprintf(stderr, "Error setting environment variable\n");
            exit(1);
        }
        
        stat = setenv(FIRST_PID, pidString, 1);
        if(stat == -1){
            fprintf(stderr, "Error setting environment variable\n");
            exit(1);
        }
    }
}

void logAction(struct logInfo *log, char *action, char *info) {
    if (!log->hasLogFile) {
        return;
    }
    pid_t pid = getpid();
    clock_t now = clock();
    fprintf(
        log->logfile,
        "%ld ; %d ; %s ; %s\n",
        now - log->startTime,
        pid,
        action,
        info);
}

void logProcessCreation(struct logInfo *log, int argc, char *argv[]) {
    char *args = argv[0];
    for (int i = 1; i < argc; i++) {
        strcat(args, " ");
        strcat(args, argv[i]);
    }
    logAction(log, "PROC_CREAT", args);
}

void logExit(struct logInfo *log, int exitStatus) {
    char exit[20];
    snprintf(exit, sizeof(exit), "%d", exitStatus);
    logAction(log, "PROC_EXIT", exit);

    unsetenv(START_TIME);
    unsetenv(FIRST_PID);
}

void logSignalReceived(struct logInfo *log, int signal) {
    char *sigName = strdup(sys_siglist[signal]);
    if (!sigName) {
        return;
    }
    char *oldPointer = sigName;
    while (*sigName) {
        *sigName = toupper(*sigName);
        sigName++;
    }

    logAction(log, "SIGNAL_RECV", sigName);
    free(oldPointer);
}

void logSignalSent(struct logInfo *log, int signal, int pid) {
    char *sigName = strdup(sys_siglist[signal]);
    if (!sigName) {
        return;
    }
    char *oldPointer = sigName;
    while (*sigName) {
        *sigName = toupper(*sigName);
        sigName++;
    }

    char sigNamePid[100];
    snprintf(sigNamePid, sizeof(sigNamePid), "%s : %d", sigName, pid);
    logAction(log, "SIGNAL_SENT", sigNamePid);
    free(oldPointer);
}

void logChangePerms(struct logInfo *log, char *path, mode_t oldPerm,
                    mode_t newPerm) {
    char perms[100];
    snprintf(perms, sizeof(perms), " : %o : %o", oldPerm, newPerm);
    strcat(path, perms);
    logAction(log, "FILE_MODF", path);
}
