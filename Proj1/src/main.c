#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <stdbool.h>

#define SIMPLE 0
#define VERBOSE 1
#define ONCHANGE 2

struct Options {
    bool octal; // octal or string?
    bool recursive;
    int output; // simple, verbose or on change?
} Options;

void changePermsWithOctal(const char *pathname, mode_t mode) {
    chmod(pathname, mode);
}

void parseMode(const char * modeString) {
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
    }
}

int main(int argc, char* argv[]) {

    if (argc < 3) {
        fprintf(stderr, "Wrong number of arguments! ");
        printf("Call the function with:\nxmod [OPTIONS] MODE FILE/DIR\n");
        exit(1);
    }

    parseMode(argv[1]);

    mode_t mode = (mode_t) atoi(argv[1]);
    changePermsWithOctal(argv[2], mode);

    return 0;
}