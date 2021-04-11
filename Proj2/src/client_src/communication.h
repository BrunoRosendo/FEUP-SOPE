#ifndef COMM_H_
#define COMM_H_

#include "aux.h"

/*
Creates a FIFO (if it doesn't exist) and waits for the server to synchronize
*/
void syncWithServer(Settings* settings);

#endif