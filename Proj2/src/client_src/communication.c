#include "communication.h"

static time_t startTime;
static int requestID = 0;
static int serverClosed = 0;
static int clientClosed = 0;
pthread_mutex_t lock;

void syncWithServer(Settings* settings) {
    // There's an error if the server already created the FIFO

    printf("Synchronizing with server...\n");
    /*
        The client is blocked until the server opens the public fifo as RONLY
        Client sends the requests here and receives answers in private fifos
    */

    time_t start = time(NULL);  // time in seconds

    // Open fifo in nonblocking mode so that it fails if still not created
    while ( (settings->fd =
        open(settings->fifoname, O_WRONLY, O_NONBLOCK) ) == -1) {
        if (time(NULL) - start > MAX_TIME_WAITING_FIFO) {
            printf("The max waiting time has been exceeded\n");
            exit(1);
        }

        usleep(TIME_BETWEEN_ATTEMPTS_FIFO);
    }

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

    // Closes all opened file descriptors
    clientClosed = 1;
    int n = sysconf(_SC_OPEN_MAX);
    for (int i = 3; i < n; i++) {
        close(i);
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
    if (read(fda, &answer, sizeof(message)) >= 0) {
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

    // Error
    } else if (clientClosed) {
        // The fifo was closed at the end of the program
        registerOperation(message.rid, message.tskload, message.pid,
            message.tid, answer.tskres, CLIENT_REQUEST_TIMEOUT);
    } else {
        // There was another error
        fprintf(stderr,
                "Unexpected error while reading from private fifo %s",
                fifoName);
        exit(1);
    }

    unlink(fifoName);
    pthread_mutex_unlock(&lock);
    return NULL;
}

void registerOperation(int rid, int tskload, int pid, pthread_t tid,
    int tskres, char* oper) {
        printf("%lu ; %d ; %d ; %d ; %lu ; %d ; %s\n", time(NULL),
            rid, tskload, pid, tid, tskres, oper);
}