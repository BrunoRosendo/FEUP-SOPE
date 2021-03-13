#include "aux.h"
#include "logs.h"

// VARIABLES NEEDED FOR LOGS AND SIGNAL HANDLING

static int nftot = 0;
static int nfmod = 0;
static char canonicPath[256];
extern logInfo logInformation;

// FUNCTIONS FOR SIGNAL HANDLING

void handleSigint(int signo) {
    logSignalReceived(&logInformation, signo);

    pid_t pid = getpid();
    printf("\n%d ; %s ; %d ; %d\n", pid, canonicPath, nftot, nfmod);

    // only one should do it. DO IT LATER
    printf("Do you want to terminate the program? (y/n)\n");
    char res = getchar();
    if (res == 'n' || res == 'N') return;
    exit(5);
}

void handleOtherSigs(int signo) {
    logSignalReceived(&logInformation, signo);
}

void subscribeSignals(char newPath[]) {
    realpath(newPath, canonicPath);  // for sigint handling
    // Subscribe SIGINT handling
    struct sigaction newInt, oldInt;
    sigset_t smask;

    if (sigemptyset(&smask) == -1) {
        perror("sigset failed\n");
        exit(5);
    }
    newInt.sa_handler = handleSigint;
    newInt.sa_mask = smask;
    newInt.sa_flags = 0;
    if (sigaction(SIGINT, &newInt, &oldInt) == -1) {
        perror("sigaction failed\n");
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
            exit(5);
        }
        new.sa_handler = handleOtherSigs;
        new.sa_mask = mask;
        new.sa_flags = 0;
        if (sigaction(i, &new, &old) == -1) {
            perror("sigaction failed\n");
            printf("%d\n", i);
            exit(5);
        }
    }
}

// FUNCTIONS FOR PARSING AND CHANGING FILE PERMISSIONS

void parseMode(const char *modeString, Options *options, char cutString[]) {
    // Check if it's in octal mode
    if (isdigit(modeString[0])) {
        for (int i = 0; modeString[i] != '\0'; ++i)
            if (modeString[i] < '0' || modeString[i] > '7') {
                fprintf(stderr, "xmod: invalid mode: '%s'\n", modeString);
                exit(2);
            }

        options->octal = true;
        options->action = substitute;
        options->user = all;
        strcpy(cutString, modeString);
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
            exit(3);
        }
    return mode;
}

// Reads permission from a file and returns an octal
mode_t getPermissionsFromFile(char* fileName) {
    struct stat fileStat;
    if (stat(fileName, &fileStat) < 0) {
        fprintf(stderr, "xmod: cannot access '%s': No such file or directory\n",
                fileName);
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


// assumes valid arguments
mode_t getOctalFromExplicitString(const char *modeString, Options *options,
                                  char* fileName) {
    mode_t mode = 0;
    for (int i = 0; i < 3; ++i) {  // other chars get ignored
        if (options->action == substitute || options->action == add) {
            switch (options->user) {
                case owner:
                    if (modeString[i] == 'r') mode |= 0400;
                    if (modeString[i] == 'w') mode |= 0200;
                    if (modeString[i] == 'x') mode |= 0100;
                    break;
                case group:
                    if (modeString[i] == 'r') mode |= 0040;
                    if (modeString[i] == 'w') mode |= 0020;
                    if (modeString[i] == 'x') mode |= 0010;
                    break;
                case others:
                    if (modeString[i] == 'r') mode |= 0004;
                    if (modeString[i] == 'w') mode |= 0002;
                    if (modeString[i] == 'x') mode |= 0001;
                    break;
                case all:
                    if (modeString[i] == 'r') mode |= 0444;
                    if (modeString[i] == 'w') mode |= 0222;
                    if (modeString[i] == 'x') mode |= 0111;
                    break;
            }
            if (options->action == add)
                mode |= getPermissionsFromFile(fileName);
        } else {  // Erase
            mode = getPermissionsFromFile(fileName);
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
                exit(3);
        }
    }
}

// assumes valid arguments
void changePermsWithOctal(const char *pathname, mode_t mode) {
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
        char name[1000];

        mode_t oldMode = getPermissionsFromFile(directoryPath);
        nftot++;

        if (dirPointer == NULL) {
            // get info about the file/folder at the path name
            lstat(directoryPath, &inode);
            if (S_ISREG(inode.st_mode)) {  // if it is a file
                changePermsWithOctal(directoryPath, mode);
                if (oldMode != mode) nfmod++;
            } else {
                fprintf(stderr, "Error opening directory\n");
            }
            return;
        }

        changePermsWithOctal(directoryPath, mode);
        if (oldMode != mode) nfmod++;

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
                "xmod: cannot access %s: Permission denied\n", name);
                continue;
            }

            // test the type of file
            if (S_ISDIR(inode.st_mode)) {
                int pid = fork();
                switch (pid) {
                    case 0:
                        argv[argc-1] = name;

                        execvp("./xmod", argv);
                        break;
                    default:
                        break;
                }
            } else if (S_ISREG(inode.st_mode)) {
                mode_t oldModeFile = getPermissionsFromFile(name);
                nftot++;
                changePermsWithOctal(name, mode);
                if (mode != oldModeFile) nfmod++;
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
            changePermsWithOctal(directoryPath, mode);
            if (mode != oldMode) nfmod++;
        } else {
            fprintf(stderr,
                "xmod: cannot access 'path': No such file or directory\n");
        }
    }
}
