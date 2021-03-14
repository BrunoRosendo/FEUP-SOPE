#include <sys/stat.h>
#include "logs.h"

void setLogFile(int argc, char *argv[], char *envp[]) {
    for (char **env = envp; *env != 0; env++)     {
        char *thisEnv = *env;
        if (strstr(thisEnv, LOGFILE) != 0) {
            // Set up the log file
            const char delim[2] = "=";
            strtok(thisEnv, delim);
            char *filename = strtok(NULL, delim);

            // if the env variable already exists
            if (atoi(getenv(FIRST_PID)) != getpid()) {
                logs.logfile = fopen(filename, "a");
            } else {
                logs.logfile = fopen(filename, "w");
            }
            logs.hasLogFile = true;

            // Save the arguments
            char args[2048];
            snprintf(args, sizeof(args), "%s", argv[0]);
            for (int i = 1; i < argc; i++) {
                snprintf(args + strlen(args), sizeof(args), " %s", argv[i]);
            }
            logs.args = (char*) malloc(sizeof(args));
            snprintf(logs.args, sizeof(args), "%s", args);

            // Start the timer for the logging
            setLogStart();
            return;
        }
    }
    logs.hasLogFile = false;
}

void setLogStart() {
    if (getenv(FIRST_PID)) {  // if the env variable already exists
        logs.startTime = atol(getenv(START_TIME));
    } else {
        struct timespec time;
        clock_gettime(CLOCK_REALTIME, &time);
        logs.startTime = time.tv_sec*1000 + time.tv_nsec/(pow(10, 6));

        char pidString[10];
        char startTimeString[50];
        snprintf(pidString, sizeof(pidString), "%d", getpid());
        snprintf(startTimeString, sizeof(startTimeString) , "%ld",
                logs.startTime);

        int stat = setenv(START_TIME, startTimeString, 1);
        if (stat == -1) {
            fprintf(stderr, "Error setting environment variable\n");
            exit(1);
        }

        stat = setenv(FIRST_PID, pidString, 1);
        if (stat == -1) {
            fprintf(stderr, "Error setting environment variable\n");
            exit(1);
        }
    }
}

void closeLogFile() {
    if (logs.hasLogFile) {
        fclose(logs.logfile);
    }
}

void logAction(char *action, char *info) {
    if (!logs.hasLogFile) {
        return;
    }
    pid_t pid = getpid();
    clock_t now = clock();
    long time = (now - logs.startTime) * 1000 / CLOCKS_PER_SEC;
    fprintf(
        logs.logfile,
        "%ld ; %d ; %s ; %s\n",
        time,
        pid,
        action,
        info);
}

void logProcessCreation() {
    logAction("PROC_CREAT", logs.args);
}

void logExit(int exitStatus) {
    char exit[20];
    snprintf(exit, sizeof(exit), "%d", exitStatus);

    logAction("PROC_EXIT", exit);
    unsetenv(START_TIME);
    unsetenv(FIRST_PID);
}

void logSignalReceived(int signal) {
    char *sigName = strdup(sys_siglist[signal]);
    if (!sigName) {
        return;
    }
    char *oldPointer = sigName;
    while (*sigName) {
        *sigName = toupper(*sigName);
        sigName++;
    }

    logAction("SIGNAL_RECV", sigName);
    free(oldPointer);
}

void logSignalSent(int signal, int pid) {
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
    logAction("SIGNAL_SENT", sigNamePid);
    free(oldPointer);
}

void logChangePerms(char *path, mode_t newPerm, mode_t oldPerms) {
    char perms[300];

    snprintf(perms, sizeof(perms), "%s : %o : %o", path, oldPerms, newPerm);
    logAction("FILE_MODF", perms);
}
