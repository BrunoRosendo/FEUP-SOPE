#include "communication.h"
#include "errno.h"

static time_t startTime;
static int requestID = 0;  // this will have to be a mutex/sephamore

void syncWithServer(Settings* settings) {
    // There's an error if the server already created the FIFO
    if (mkfifo(settings->fifoname, FIFO_PUBLIC_PERMS)) {
        printf("Mkfifo error: %d\n", errno);
        exit(2);
    }

    printf("Synchronizing with server...\n");
    /*
        The client waits for the server to open the public fifo as WRONLY
        Client sends the requests here and receives answers in private fifos
    */
    while ((settings->fd = open(settings->fifoname, O_WRONLY)) < 0) {
        usleep(SYNC_WAIT_TIME);
    }
    printf("Server synchronized with success\n");
}

void generateRequests(Settings* settings) {
    time(&startTime);  // sets the start of the requests

    while (1) {
        pthread_t tid;
        pthread_create(&tid, NULL, makeRequest, (void*)&settings->fd);
        pthread_join(tid, NULL);

        if (time(NULL) - startTime >= settings->execTime) break;

        int waitTime = rand() % 100 + 1;  // milliseconds
        usleep(waitTime);
    }
}

void *makeRequest(void* arg) {
    int *fd = (int *) arg;
    pthread_t tid = pthread_self();
    return NULL;
}
