#ifndef PARSING_H
#define PARSING_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <ctype.h>
#include <sys/stat.h>

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

void parseMode(const char *modeString, Options *options, char cutString[]);


// Genereate octacl from octal string.
mode_t getOctalFromOctalString(char *modeString);

// Reads permission from a file and returns an octal
mode_t getPermissionsFromFile(char* filename)

// Generate octal from string. Return octal
mode_t getOctalFromExplicitString(const char *modeString, Options *options, char *filename);

void parseFlag(char *flag, Options *options);

#endif
