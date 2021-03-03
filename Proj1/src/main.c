#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <ctype.h>

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
void changePermsWithString(const char *pathname, const char *modeString, Options *options) {
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
    changePermsWithOctal(pathname, mode);
}

void parseMode(const char *modeString, Options *options, char* cutString) {
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
        return;
    }

    // Start parsing the string mode

    int i = 0;
    options->user = all;  // default
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
    i++; int j; // WATCH OUT
    for (j = 0; modeString[i] != '\0'; ++i, ++j)
        cutString[j] = modeString[i];
    ++j;
    cutString[j] = '\0';
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Wrong number of arguments! ");
        printf("Call the function with:\nxmod [OPTIONS] MODE FILE/DIR\n");
        exit(1);
    }

    Options options;
    char modeString[10];
    parseMode(argv[argc - 2], &options, &modeString);

    mode_t mode = (mode_t) atoi(argv[1]);
    changePermsWithOctal(argv[2], mode);

    return 0;
}
