#include "communication.h"

static time_t startTime;
static int requestID = 0;  // this will have to be a mutex/sephamore
// static int res = -2;
static int serverClosed = 0;
pthread_mutex_t lock;

void syncWithServer(Settings* settings) {
    // There's an error if the server already created the FIFO
    if (mkfifo(settings->fifoname, FIFO_PUBLIC_PERMS) && errno != EEXIST) {
        printf("Mkfifo error: %d\n", errno);
        exit(2);
    }

    printf("Synchronizing with server...\n");
    /*
        The client is blocked until the server opens the public fifo as RONLY
        Client sends the requests here and receives answers in private fifos
    */
    settings->fd = open(settings->fifoname, O_WRONLY);
    printf("Server synchronized with success\n");
}

void generateRequests(Settings* settings) {
    time(&startTime);  // sets the start of the requests

    if (pthread_mutex_init(&lock, NULL)) {
        fprintf(stderr, "Erros initializing mutex\n");
        exit(3);
    }

    while (1) {
        pthread_t tid;
        pthread_create(&tid, NULL, makeRequest, (void*)&settings->fd);
        // by detatching, the thread joins when it terminates
        pthread_detach(tid);

        if (time(NULL) - startTime >= settings->execTime || serverClosed)
            break;

        int waitTime = 1000 * (rand() % 100 + 1);  // milliseconds
        usleep(waitTime);
    }
    pthread_mutex_destroy(&lock);
}

void *makeRequest(void* arg) {
    pthread_mutex_lock(&lock);

    // Get file descriptor
    int *fd = (int *) arg;

    // Build message struct
    Message message;
    message.tid = pthread_self();  // or syscall(SYS_gettid) ?
    message.rid = requestID++;
    message.pid = getpid();
    message.tskload = rand() % 9 + 1;
    message.tskres = -1;

    // Create private fifo
    char fifoName[MAX_PATH_SIZE];
    snprintf(fifoName, MAX_PATH_SIZE, "/tmp/%d.%lu", message.pid, message.tid);
    if (mkfifo(fifoName, FIFO_PUBLIC_PERMS)) {
        printf("Mkfifo error: %d\n", errno);
        exit(2);
    }

    // Send request
    write(*fd, &message, sizeof(message));

    registerOperation(message.rid, message.tskload, message.pid,
        message.tid, message.tskres, CLIENT_WANTS);

    // Get answer
    Message answer;
    int fda = open(fifoName, O_RDONLY);

    read(fda, &answer, sizeof(message));

    if (answer.tskres == -1) {
        registerOperation(message.rid, message.tskload, message.pid,
            message.tid, answer.tskres, CLIENT_REQUEST_CLOSED);

        serverClosed = 1;
    } else {
        registerOperation(message.rid, message.tskload, message.pid,
            message.tid, answer.tskres, CLIENT_RCVD);
    }


    // Delete private fifo
    close(fda);
    unlink(fifoName);

    pthread_mutex_unlock(&lock);
    return NULL;
}

void registerOperation(int rid, int tskload, int pid, pthread_t tid,
    int tskres, char* oper) {
        printf("%lu ; %d ; %d ; %d ; %lu ; %d ; %s\n", time(NULL),
            rid, tskload, pid, tid, tskres, oper);
}
