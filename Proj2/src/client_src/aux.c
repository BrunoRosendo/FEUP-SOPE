#include "aux.h"

void parseCMDArgs(char* argv[], Settings* settings) {

}

void syncWithServer(Settings* settings) {
    // There's an error if the server already created the FIFO
    mkfifo(settings->fifoname, FIFO_PUBLIC_PERMS);

    // sync the client with the server
    while (open(settings->fifoname, O_RDWR) < 0) {}
}

void init(char* argv[], Settings* settings) {
    parseCMDArgs(argv, settings);
    syncWithServer(settings);
}
