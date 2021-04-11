#include "communication.h"

void syncWithServer(Settings* settings) {
    // There's an error if the server already created the FIFO
    mkfifo(settings->fifoname, FIFO_PUBLIC_PERMS);

    printf("Synchronizing with server...\n");
    /*
        The client waits for the server to open the public fifo as WRONLY
        Client sends the requests here and receives answers in private fifos
    */
    while (open(settings->fifoname, O_RDONLY) < 0) {
        usleep(SYNC_WAIT_TIME);
    }
    printf("Server synchronized with success\n");
}
