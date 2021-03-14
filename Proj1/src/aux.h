#ifndef AUX_H_
#define AUX_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <ctype.h>
#include <sys/wait.h>
#include <sys/types.h>
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


void handleSigint(int signo);

void handleOtherSigs(int signo);

void subscribeSignals(char newPath[]);

void parseMode(char *modeString, Options *options, char cutString[]);

/**
 * Generate octal from octal string
 * @return octal
 */
mode_t getOctalFromOctalString(char *modeString);

/**
 * Generate explicit string from octal
*/
void getExcplicitStringFromOctal(char *permissionInString, mode_t octalP);

/**
 * Handles permission changes
*/
void handleChangedPermissions(char *filePath, mode_t mode, mode_t oldMode,
                                Options *options);

/**
 * Reads the permissions from a file
 * @return octal
 */
mode_t getPermissionsFromFile(char* fileName);

/**
 * Generate octal from string
 * @return octal
 *  */
mode_t getOctalFromExplicitString(char *modeString, Options *options,
                                  char* fileName);

void parseFlag(char *flag, Options *options);

void changePermsWithOctal(char *pathname, mode_t mode, mode_t oldMode);

void applyToPath(char *directoryPath, mode_t mode, Options *options,
                  int argc, char **argv);

#endif
