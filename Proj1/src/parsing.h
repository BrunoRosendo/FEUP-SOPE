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


/**
 * Generate octal from octal string
 * @return octal
 */
mode_t getOctalFromOctalString(char *modeString);

/**
 * Reads the permissions from a file
 * @return octal
 */
mode_t getPermissionsFromFile(char* fileName);

/**
 * Generate octal from string
 * @return octal
 *  */
mode_t getOctalFromExplicitString(const char *modeString, Options *options, char* fileName);

void parseFlag(char *flag, Options *options);

#endif
