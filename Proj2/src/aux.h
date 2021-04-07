#ifndef AUX_H_
#define AUX_H_

#include <stdio.h>
#include <stdlib.h>

typedef struct Settings {
    unsigned execTime;
    char* fifoname;
} Settings;

void parseCMDArgs(char* argv[], Settings* settings);

#endif  // AUX_H_
