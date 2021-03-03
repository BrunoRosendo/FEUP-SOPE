#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <ctype.h>
#include <dirent.h>

typedef enum outputOption {simple, verbose, onChange} outputOption;
typedef enum userType {owner, group, others, all} userType;
typedef enum permAction {erase, add, substitute} permAction;

typedef struct Options {
    bool octal;  // octal or string?
    bool recursive;
    outputOption output;
    userType user;
    permAction action;
} Options;

// assumes valid arguments
void changePermsWithOctal(const char *pathname, mode_t mode) {
    chmod(pathname, mode);
}

// assumes valid arguments
mode_t changePermsWithString(const char *pathname, const char *modeString, Options *options) {
    mode_t mode = 0;
    switch (options->user) {
        case owner:
            if (modeString[0] == 'r')
                mode |= 0400;
            if (modeString[1] == 'w')
                mode |= 0200;
            if (modeString[2] == 'x')
                mode |= 0100;
            break;
        case group:
            if (modeString[0] == 'r')
                mode |= 0040;
            if (modeString[1] == 'w')
                mode |= 0020;
            if (modeString[2] == 'x')
                mode |= 0010;
            break;
        case others:
            if (modeString[0] == 'r')
                mode |= 0004;
            if (modeString[1] == 'w')
                mode |= 0002;
            if (modeString[2] == 'x')
                mode |= 0001;
            break;
        case all:
            if (modeString[0] == 'r')
                mode |= 0400;
            if (modeString[1] == 'w')
                mode |= 0200;
            if (modeString[2] == 'x')
                mode |= 0100;
            if (modeString[3] == 'r')
                mode |= 0040;
            if (modeString[4] == 'w')
                mode |= 0020;
            if (modeString[5] == 'x')
                mode |= 0010;
            if (modeString[6] == 'r')
                mode |= 0004;
            if (modeString[7] == 'w')
                mode |= 0002;
            if (modeString[8] == 'x')
                mode |= 0001;
            break;
        default:
            fprintf(stderr, "Invalid userType\n");
            exit(3);
    }
    //changePermsWithOctal(pathname, mode);
    return mode;
}

void parseMode(const char *modeString, Options *options, char cutString[]) {
    // Check if it's in octal mode
    if (strlen(modeString) == 4) {
        bool valid = true;
        if (modeString[0] != '0') valid = false;

        for (int i = 1; i < 4; ++i)
            if (modeString[i] < '0' || modeString[i] > '7') valid = false;

        if (!valid) {
            fprintf(stderr, "Wrong format of OCTAL-MODE:\n");
            printf("Use the format 0XXX where X is between 0 and 7\n");
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
                fprintf(stderr, "Invalid input (userType)\n");
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
            fprintf(stderr, "Invalid input (permission action)\n");
            exit(3);
    }
    printf("Complete: %s\n", modeString);
    i++; int j;  // REDO THIS LATER
    for (j = 0; modeString[i] != '\0'; ++i, ++j)
        cutString[j] = modeString[i];
    ++j;
    cutString[j] = 0;
    printf("Cut: %s\n", cutString);
}

mode_t getOctalFromString(char *modeString)
{
    mode_t mode = 0;

    switch (modeString[1])
    {
    case '0':
        break;
    case '1':
        mode |= 0100;
        break;
    case '2':
        mode |= 0200;
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
        fprintf(stderr, "Invalid input\n");
        exit(3);
    }

    switch (modeString[2])
    {
    case '0':
        break;
    case '1':
        mode |= 0010;
        break;
    case '2':
        mode |= 0020;
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
        fprintf(stderr, "Invalid input\n");
        exit(3);
    }

    switch (modeString[3])
    {
    case '0':
        break;
    case '1':
        mode |= 0001;
        break;
    case '2':
        mode |= 0002;
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
        fprintf(stderr, "Invalid input\n");
        exit(3);
    }
    return mode;
}

void applyToDirectory(char* directoryPath, mode_t mode){
    // opens a directory. Returns a valid pointer if the successfully, NULL otherwise
    DIR *dirPointer = opendir(directoryPath);

    struct dirent *dirEntry;
    struct stat inode;
    char name[1000];

    if (dirPointer == NULL){
        lstat(directoryPath, &inode);                                     // get info about the file/folder at the path name
        if(S_ISREG(inode.st_mode)){     // if it is a file
            changePermsWithOctal(directoryPath, mode);
        }
        else{
            printf("Error opening directory\n");
        }
        return;
    }

    while ((dirEntry = readdir(dirPointer)) != 0)
    {
        sprintf(name, "%s/%s", directoryPath, dirEntry->d_name); // sends formatted output to a string(name) / name will be the absolute path to the next file
        lstat(name, &inode);                                     // get info about the file/folder at the path name

        // test the type of file
        if (S_ISDIR(inode.st_mode))
            printf("dir ");
        else if (S_ISREG(inode.st_mode)){
            printf("fis ");
            changePermsWithOctal(name, mode);
        }
        else if (S_ISLNK(inode.st_mode))
            printf("lnk ");
        else
            ;
        printf(" %s\n", dirEntry->d_name);
    }
}

int main(int argc, char* argv[], char* envp[]) {
    if (argc < 3) {
        fprintf(stderr, "Wrong number of arguments! ");
        printf("Call the function with:\nxmod [OPTIONS] MODE FILE/DIR\n");
        exit(1);
    }

    Options options;
    char modeString[10];
    parseMode(argv[argc - 2], &options, modeString);
    mode_t mode;
    if (options.octal) {
        mode = getOctalFromString(modeString);
        //changePermsWithOctal(argv[argc - 1], mode);
    } else {
        mode = changePermsWithString(argv[argc - 1], modeString, &options);
    }

    applyToDirectory(argv[argc-1], mode);
    
    return 0;
}

