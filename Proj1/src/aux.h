#ifndef AUX_H_
#define AUX_H_

// System includes
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h>

// C libraries includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <ctype.h>
#include <dirent.h>

// Constants and structs
#define MAX_FILEPATH_SIZE 500

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

/**
 * Handles an interruptions signal
 */
void handleSigint(int signo);

/**
 * Handles all signals throu 1-31 except
 * SIGINT, SIGKILL AND SIGSTOP
 */
void handleOtherSigs(int signo);

/**
 * Subscribes all signals to their functions
 * except SIGKILL AND SIGSTOP
 */
void subscribeSignals(char newPath[]);

/**
 * Parses a string mode, updates the options
 * and saves the important part to cutString
 */
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

/**
 * Parses a flag and updates the options
 * Accepts multiple flags such as -Rv
 */
void parseFlag(char *flag, Options *options);

/**
 * Changes permissions with a mode_t variable
 * and logs it.
 * Assumes valid arguments
 */
void changePermsWithOctal(char *pathname, mode_t mode, mode_t oldMode);

/**
 * Takes a mode_t variable and applies it
 * according to the options
 */
void applyToPath(char *directoryPath, mode_t mode, Options *options,
                  int argc, char **argv);

#endif
