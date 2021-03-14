#include "logs.h"

void setLogFile() {
    char *fileName = getenv(LOGFILE);
    if (fileName) {
        // if the env variable already exists
        if (atoi(getenv(FIRST_PID)) != getpid()) {
            logs.logfile = fopen(fileName, "a");
        } else {
            logs.logfile = fopen(fileName, "w");  // erase content
            logs.logfile = freopen(fileName, "a", logs.logfile);
        }

        logs.hasLogFile = true;
        return;
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
    struct timespec timeStruct;
    clock_gettime(CLOCK_REALTIME, &timeStruct);
    long now = timeStruct.tv_sec * 1000 + timeStruct.tv_nsec / (pow(10, 6));
    long time = now - logs.startTime;
    fprintf(
        logs.logfile,
        "%ld ; %d ; %s ; %s\n",
        time,
        pid,
        action,
        info);
}

void logProcessCreation(int argc, char* argv[]) {
    char args[2048];
    snprintf(args, sizeof(args), "%s", argv[0]);
    for (int i = 1; i < argc; i++) {
        snprintf(args + strlen(args), sizeof(args), " %s", argv[i]);
    }
    logAction("PROC_CREAT", args);
}

void logExit(int exitStatus) {
    char exit[20];
    snprintf(exit, sizeof(exit), "%d", exitStatus);

    logAction("PROC_EXIT", exit);
    unsetenv(START_TIME);
    unsetenv(FIRST_PID);
}

void logSignalReceived(int signal) {
    logAction("SIGNAL_RECV", strsignal(signal));
}

void logSignalSent(int signal, int pid) {
    char *sigName = strsignal(signal);

    char sigNamePid[100];
    snprintf(sigNamePid, sizeof(sigNamePid), "%s : %d", sigName, pid);
    logAction("SIGNAL_SENT", sigNamePid);
}

void logChangePerms(char *path, mode_t newPerm, mode_t oldPerms) {
    char perms[300];

    snprintf(perms, sizeof(perms), "%s : %o : %o", path, oldPerms, newPerm);
    logAction("FILE_MODF", perms);
}
