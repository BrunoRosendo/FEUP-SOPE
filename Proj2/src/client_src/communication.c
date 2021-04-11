#include "communication.h"

void syncWithServer(Settings* settings) {
    // There's an error if the server already created the FIFO
    mkfifo(settings->fifoname, FIFO_PUBLIC_PERMS);

    // sync the client with the server
    while (open(settings->fifoname, O_RDWR) < 0) {}
}
