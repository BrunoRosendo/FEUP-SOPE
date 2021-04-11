#include "communication.h"

static time_t startTime;
static int requestID = 0;  // this will have to be a mutex/sephamore

void syncWithServer(Settings* settings) {
    // There's an error if the server already created the FIFO
    if (mkfifo(settings->fifoname, FIFO_PUBLIC_PERMS) && errno != EEXIST) {
        printf("Mkfifo error: %d\n", errno);
        exit(2);
    }

    printf("Synchronizing with server...\n");
    /*
        The client is blocked until the server opens the public fifo as WRONLY
        Client sends the requests here and receives answers in private fifos
    */
    settings->fd = open(settings->fifoname, O_WRONLY);
    printf("Server synchronized with success\n");
}

void generateRequests(Settings* settings) {
    time(&startTime);  // sets the start of the requests

    while (1) {
        pthread_t tid;
        pthread_create(&tid, NULL, makeRequest, (void*)&settings->fd);
        void* tmp;
        pthread_join(tid, &tmp);
        int *res = (int *) tmp;

        if (time(NULL) - startTime >= settings->execTime || *res == -1)
            break;

        int waitTime = rand() % 100 + 1;  // milliseconds
        usleep(waitTime);
    }
}

// we will need mutexes/sephamores for this. It's not working correctly yet
void *makeRequest(void* arg) {
    int *fd = (int *) arg;

    Message message;
    message.tid = pthread_self();
    message.rid = requestID++;
    message.pid = getpid();
    message.tskload = rand() % 9 + 1;

    // Create private fifo
    char fifoName[MAX_PATH_SIZE];
    snprintf(fifoName, MAX_PATH_SIZE, "/tmp/%d.%lu", message.pid, message.tid);
    if (mkfifo(fifoName, FIFO_PUBLIC_PERMS)) {
        printf("Mkfifo error: %d\n", errno);
        exit(2);
    }

    // Send request
    char request[MAX_REQUEST_SIZE];
    snprintf(request, sizeof(request), "%d %d %d %lu -1",
        message.rid,
        message.tskload,
        message.pid,
        message.tid);
    write(*fd, request, strlen(request) + 1);

    // Get answer
    char answer[MAX_REQUEST_SIZE];
    int fda = open(fifoName, O_RDONLY);  // not working very well :/ Is server not writing?
    read(fda, answer, sizeof(answer));

    int res;
    char* split = answer;
    for (int i = 0; i < 4; ++i)
        split = strtok(split, " ");
    res = atoi(split);

    return res;  // change this to put the result in a global variable
}
