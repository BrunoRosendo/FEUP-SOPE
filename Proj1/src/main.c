#include "parsing.h"

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

int main(int argc, char* argv[], char* envp[]) {
    if (argc < 3) {
        fprintf(stderr, "Wrong number of arguments! ");
        printf("Call the function with:\nxmod [OPTIONS] MODE FILE/DIR\n");
        exit(1);
    }

    Options options;
    char modeString[10];
    parseMode(argv[argc - 2], &options, modeString);

    if (options.octal) {
        mode_t mode = getOctalFromString(modeString);
        changePermsWithOctal(argv[argc - 1], mode);
    } else {
        changePermsWithString(argv[argc - 1], modeString, &options);
    }

    return 0;
}
