#include "logs.h"
#include "aux.h"

// VARIABLES NEEDED FOR LOGS AND SIGNAL HANDLING

static int waitingForSig = 1;
static int nftot = 0;
static int nfmod = 0;

char *canonicPath = NULL;  // this will be malloc'ed by realpath()

// FUNCTIONS FOR SIGNAL HANDLING

void handleSigint(int signo) {
    logSignalReceived(signo);

    pid_t pid = getpid();
    printf("%d ; %s ; %d ; %d\n", pid, canonicPath, nftot, nfmod);
    pid_t firstpid = atoi(getenv(FIRST_PID));

    usleep(10000);
    if (firstpid == getpid()) {
        printf("Do you want to terminate the program? (y/n)\n");

        char buffer[20];  // for multiple CTRL+C
        scanf("%s", buffer);

        if (toupper(buffer[0]) == 'Y') {
            logSignalSent(SIGUSR2, firstpid);
            killpg(firstpid, SIGUSR2);
        } else {
            logSignalSent(SIGUSR1, firstpid);
            killpg(firstpid, SIGUSR1);
        }

    } else {
        while (waitingForSig) {}  // wait for a decision
        waitingForSig = 1;
    }
}

void handleOtherSigs(int signo) {
    logSignalReceived(signo);
    if (signo == SIGUSR1) waitingForSig = 0;
    if (signo == SIGUSR2) {
        logExit(6);
        exit(6);
    }
}

void subscribeSignals(char newPath[]) {
    canonicPath = realpath(newPath, NULL);  // for sigint handling
    // Subscribe SIGINT handling
    struct sigaction newInt, oldInt;
    sigset_t smask;

    if (sigemptyset(&smask) == -1) {
        perror("sigset failed\n");
        logExit(5);
        exit(5);
    }
    newInt.sa_handler = handleSigint;
    newInt.sa_mask = smask;
    newInt.sa_flags = 0;
    if (sigaction(SIGINT, &newInt, &oldInt) == -1) {
        perror("sigaction failed\n");
        logExit(5);
        exit(5);
    }

    // Subscribe handling to other possible signals
    for (int i = 1; i <= 31; ++i) {
        if (i == SIGINT || i == SIGSTOP || i == SIGKILL)
            continue;  // already handled or impossible to
        struct sigaction new, old;
        sigset_t mask;
        if (sigemptyset(&mask) == -1) {
            perror("sigset failed\n");
            logExit(5);
            exit(5);
        }
        new.sa_handler = handleOtherSigs;
        new.sa_mask = mask;
        new.sa_flags = 0;
        if (sigaction(i, &new, &old) == -1) {
            perror("sigaction failed\n");
            logExit(5);
            exit(5);
        }
    }
}

// FUNCTIONS FOR PARSING AND CHANGING FILE PERMISSIONS

void parseMode(char *modeString, Options *options, char cutString[]) {
    // Check if it's in octal mode
    if (isdigit(modeString[0])) {
        for (int i = 0; modeString[i] != '\0'; ++i)
            if (modeString[i] < '0' || modeString[i] > '7') {
                fprintf(stderr, "xmod: invalid mode: '%s'\n", modeString);
                logExit(2);
                exit(2);
            }

        options->octal = true;
        options->action = substitute;
        options->user = all;
        snprintf(cutString, sizeof(modeString), "%s", modeString);
        return;
    }

    // Start parsing the string mode

    int i = 0;
    options->user = all;  // default
    options->octal = false;
    if (isalpha(modeString[0])) {  // userType specified
        ++i;
        switch (modeString[0]) {
            case 'u':
                options->user = owner;
                break;
            case 'g':
                options->user = group;
                break;
            case 'o':
                options->user = others;
                break;
            case 'a':
                break;
            default:
                fprintf(stderr, "xmod: invalid mode: '%s'\n", modeString);
                logExit(3);
                exit(3);
        }
    }

    switch (modeString[i]) {
        case '-':
            options->action = erase;
            break;
        case '+':
            options->action = add;
            break;
        case '=':
            options->action = substitute;
            break;
        default:
            fprintf(stderr, "xmod: invalid mode: '%s'\n", modeString);
            logExit(3);
            exit(3);
    }
    i++;
    int j;
    for (j = 0; modeString[i] != 0; ++i, ++j)
        cutString[j] = modeString[i];
    cutString[j] = 0;
}

mode_t getOctalFromOctalString(char *modeString) {
    mode_t mode = 0;

    int i = 1;
    // 0 was not specified in the argument
    if (modeString[0] != '0') i--;

    switch (modeString[i]) {
        case '0':
            break;
        case '1':
            mode |= 0100;
            break;
        case '2':
            mode |= 0200;
            break;
        case '3':
            mode |= 0300;
            break;
        case '4':
            mode |= 0400;
            break;
        case '5':
            mode |= 0500;
            break;
        case '6':
            mode |= 0600;
            break;
        case '7':
            mode |= 0700;
            break;
        default:
            fprintf(stderr, "xmod: invalid mode: '%s'\n", modeString);
            logExit(3);
            exit(3);
    }
    ++i;

    switch (modeString[i]) {
        case '0':
            break;
        case '1':
            mode |= 0010;
            break;
        case '2':
            mode |= 0020;
            break;
        case '3':
            mode |= 0030;
            break;
        case '4':
            mode |= 0040;
            break;
        case '5':
            mode |= 0050;
            break;
        case '6':
            mode |= 0060;
            break;
        case '7':
            mode |= 0070;
            break;
        default:
            fprintf(stderr, "xmod: invalid mode: '%s'\n", modeString);
            logExit(3);
            exit(3);
    }
    ++i;

    switch (modeString[i]) {
        case '0':
            break;
        case '1':
            mode |= 0001;
            break;
        case '2':
            mode |= 0002;
            break;
        case '3':
            mode |= 0003;
            break;
        case '4':
            mode |= 0004;
            break;
        case '5':
            mode |= 0005;
            break;
        case '6':
            mode |= 0006;
            break;
        case '7':
            mode |= 0007;
            break;
        default:
            fprintf(stderr, "xmod: invalid mode: '%s'\n", modeString);
            logExit(3);
            exit(3);
        }
    return mode;
}

void getExcplicitStringFromOctal(char *permissionInString, mode_t octalP) {
    snprintf(permissionInString, sizeof("---------"), "---------");

    if (octalP & S_IRUSR)
        permissionInString[0]= 'r';
    if (octalP & S_IWUSR)
        permissionInString[1]= 'w';
    if (octalP & S_IXUSR)
        permissionInString[2]= 'x';
    if (octalP & S_IRGRP)
        permissionInString[3]= 'r';
    if (octalP & S_IWGRP)
        permissionInString[4]= 'w';
    if (octalP & S_IXGRP)
        permissionInString[5]= 'x';
    if (octalP & S_IROTH)
        permissionInString[6]= 'r';
    if (octalP & S_IWOTH)
        permissionInString[7]= 'w';
    if (octalP & S_IXOTH)
        permissionInString[8]= 'x';
}


void handleChangedPermissions(char *filePath, mode_t mode, mode_t oldMode,
                                Options *options) {
    if (mode != oldMode) {
        nfmod++;
        if (options->output == verbose || options->output == onChange) {
            char explicitStringMode[10];
            char explicitStringModeOld[10];
            getExcplicitStringFromOctal(explicitStringMode, mode);
            getExcplicitStringFromOctal(explicitStringModeOld, oldMode);
            printf("mode of '%s' changed from 0%o (%s) to 0%o (%s) \n",
                filePath, oldMode, explicitStringModeOld,
                mode, explicitStringMode);
        }
    } else if (options->output == verbose) {
        char explicitStringMode[10];
        getExcplicitStringFromOctal(explicitStringMode, mode);
        printf("mode of '%s' retained as 0%o (%s) \n",
                filePath, mode, explicitStringMode);
     }
}

// Reads permission from a file and returns an octal
mode_t getPermissionsFromFile(char* fileName) {
    struct stat fileStat;
    if (stat(fileName, &fileStat) < 0) {
        fprintf(stderr, "xmod: cannot access '%s': No such file or directory\n",
                fileName);
        logExit(4);
        exit(4);
    }

    mode_t mode = 0;

    if (fileStat.st_mode & S_IRUSR)
        mode |= 0400;

    if (fileStat.st_mode & S_IWUSR)
        mode |= 0200;

    if (fileStat.st_mode & S_IXUSR)
        mode |= 0100;

    if (fileStat.st_mode & S_IRGRP)
        mode |= 0040;

    if (fileStat.st_mode & S_IWGRP)
        mode |= 0020;

    if (fileStat.st_mode & S_IXGRP)
        mode |= 0010;

    if (fileStat.st_mode & S_IROTH)
        mode |= 0004;

    if (fileStat.st_mode & S_IWOTH)
        mode |= 0002;

    if (fileStat.st_mode & S_IXOTH)
        mode |= 0001;

    return mode;
}

mode_t getOctalFromExplicitString(char *modeString, Options *options,
                                  char* fileName) {
    mode_t mode = 0;
    mode_t oldMode = getPermissionsFromFile(fileName);
    if (options->action == erase || options->action == add) mode = oldMode;

    for (int i = 0; i < 3; ++i) {  // other chars get ignored
        if (options->action == substitute || options->action == add) {
            switch (options->user) {
                case owner:
                    if (modeString[i] == 'r') mode |= 0400;
                    if (modeString[i] == 'w') mode |= 0200;
                    if (modeString[i] == 'x') mode |= 0100;
                    if (options->action == substitute)
                        mode |= (oldMode & 0077);
                    break;
                case group:
                    if (modeString[i] == 'r') mode |= 0040;
                    if (modeString[i] == 'w') mode |= 0020;
                    if (modeString[i] == 'x') mode |= 0010;
                    if (options->action == substitute)
                        mode |= (oldMode & 0707);
                    break;
                case others:
                    if (modeString[i] == 'r') mode |= 0004;
                    if (modeString[i] == 'w') mode |= 0002;
                    if (modeString[i] == 'x') mode |= 0001;
                    if (options->action == substitute)
                        mode |= (oldMode & 0770);
                    break;
                case all:
                    if (modeString[i] == 'r') mode |= 0444;
                    if (modeString[i] == 'w') mode |= 0222;
                    if (modeString[i] == 'x') mode |= 0111;
                    break;
            }
        } else {  // Erase
            switch (options->user) {
                case owner:
                    if (modeString[i] == 'r') mode &= 0377;
                    if (modeString[i] == 'w') mode &= 0577;
                    if (modeString[i] == 'x') mode &= 0677;
                    break;
                case group:
                    if (modeString[i] == 'r') mode &= 0737;
                    if (modeString[i] == 'w') mode &= 0757;
                    if (modeString[i] == 'x') mode &= 0767;
                    break;
                case others:
                    if (modeString[i] == 'r') mode &= 0773;
                    if (modeString[i] == 'w') mode &= 0775;
                    if (modeString[i] == 'x') mode &= 0776;
                    break;
                case all:
                    if (modeString[i] == 'r') mode &= 0333;
                    if (modeString[i] == 'w') mode &= 0555;
                    if (modeString[i] == 'x') mode &= 0666;
                    break;
            }
        }
    }
    return mode;
}

void parseFlag(char *flag, Options *options) {
    if (flag[0] != '-') {
        fprintf(stderr, "xmod: invalid mode: '%s'\n", flag);
        exit(3);
    }

    for (int i = 1; flag[i] != 0; ++i) {
        switch (flag[i]) {
            case 'v':
                options->output = verbose;
                break;
            case 'c':
                options->output = onChange;
                break;
            case 'R':
                options->recursive = true;
                break;
            default:
                fprintf(stderr, "xmod: invalid options -- '%c'\n", flag[i]);
                logExit(3);
                exit(3);
        }
    }
}

void changePermsWithOctal(char *pathname, mode_t mode, mode_t oldMode) {
    if (mode != oldMode)
        logChangePerms(pathname, mode, oldMode);
    chmod(pathname, mode);
}

void applyToPath(char *directoryPath, mode_t mode, Options *options,
                  int argc, char **argv) {
    if (options->recursive) {
        // opens a directory. Returns a valid pointer if the successfully,
        // NULL otherwise
        DIR *dirPointer = opendir(directoryPath);

        struct dirent *dirEntry;
        struct stat inode;
        char name[MAX_FILEPATH_SIZE];

        mode_t oldMode = getPermissionsFromFile(directoryPath);
        nftot++;


        if (dirPointer == NULL) {
            // get info about the file/folder at the path name
            lstat(directoryPath, &inode);
            if (S_ISREG(inode.st_mode)) {  // if it is a file
                changePermsWithOctal(directoryPath, mode, oldMode);
                handleChangedPermissions(directoryPath, mode, oldMode, options);
            } else {
                fprintf(stderr, "Error opening directory \n");
            }
            return;
        }

        changePermsWithOctal(directoryPath, mode, oldMode);
        handleChangedPermissions(directoryPath, mode, oldMode, options);

        while ((dirEntry = readdir(dirPointer)) != 0) {
            // sends formatted output to a string(name) / name
            // will be the absolute path to the next file

            if (strcmp(dirEntry->d_name, ".") == 0 ||
                strcmp(dirEntry->d_name, "..") == 0)
                    continue;

            snprintf(name, sizeof(name), "%s/%s", directoryPath,
                dirEntry->d_name);

            // get info about the file/folder at the path name
            int statRet = lstat(name, &inode);
            if (statRet == -1) {
                fprintf(stderr,
                "xmod: cannot access %s: Permission denied \n", name);
                continue;
            }

            // test the type of file
            if (S_ISDIR(inode.st_mode)) {
                argv[argc-1] = name;
                int pid = fork();
                switch (pid) {
                    case 0:
                        execvp("./xmod", argv);
                        break;
                    default:
                        logProcessCreation(argc, argv);
                        break;
                }
            } else if (S_ISREG(inode.st_mode)) {
                mode_t oldMode = getPermissionsFromFile(name);
                nftot++;
                changePermsWithOctal(name, mode, oldMode);
                handleChangedPermissions(name, mode, oldMode, options);
            }
        }

        int stat_loc;
        pid_t wpid;
        // Waits for all child processes to end
        while ((wpid = wait(&stat_loc)) > 0) {}

        if (dirPointer != NULL) {
            closedir(dirPointer);
        }
    } else {  // apply to the folder
        mode_t oldMode = getPermissionsFromFile(directoryPath);
        struct stat inode;
        // get info about the file/folder at the path name
        lstat(directoryPath, &inode);
        if (S_ISDIR(inode.st_mode) || S_ISREG(inode.st_mode)) {
            nftot++;
            changePermsWithOctal(directoryPath, mode, oldMode);
            handleChangedPermissions(directoryPath, mode, oldMode, options);
        } else {
            fprintf(stderr,
                "xmod: cannot access 'path': No such file or directory\n");
        }
    }
}
