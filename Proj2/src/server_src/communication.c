#include "communication.h"

static time_t startTime;

static int numResults = 0;
static Message* buffer;
static pthread_t *threads;

static sem_t semaphore;


void listenAndRespond(Settings* settings) {
    setupForLoop(settings->bufferSize);
    int i = 0;

    while (1) {
        if (time(NULL) - startTime > settings->execTime) {
            fprintf(stderr, "[server] Execution time reached. Exiting...\n");
            break;
        }

        getNewRequest(&i, settings->fifoname);
        dispatchResults();
    }
    exitLoop(i);
}

void *processRequest(void* arg) {
    sem_wait(&semaphore);
    Message* request = (Message*) arg;

    /* CALCULATE RES FROM LIB AND PUT IN BUFFER */

    free(request);
    sem_post(&semaphore);
    return NULL;
}

void dispatchResults() {
    // TODO: CHECK IF THIS IS RIGHT
    sem_wait(&semaphore);

    for (int i = 0; i < numResults; ++i) {
        Message message = buffer[i];

        char fifoName[MAX_PATH_SIZE];
        snprintf(fifoName, MAX_PATH_SIZE, "/tmp/%d.%lu",
                message.pid, message.tid);

        int fd = open(fifoName, O_WRONLY);
        message.pid = getpid();
        message.tid = pthread_self();
        write(fd, &message, sizeof(Message));
    }

    numResults = 0;
    sem_post(&semaphore);
}

void getNewRequest(int* i, char* fifoName) {
    int fd = open(fifoName, O_RDONLY, O_NONBLOCK);
    if (fd == -1) {
        usleep(TIME_BETWEEN_ATTEMPTS_FIFO);
        return;
    }

    Message* request = (Message*) malloc(sizeof(Message));
    if (read(fd, request, sizeof(Message)) >= 0) {
        // Success
        pthread_create(&threads[*i], NULL, processRequest, (void*)request);

        ++(*i);
        threads = (pthread_t*) realloc(threads, ((*i) + 1) * sizeof(pthread_t));
    } else {
        // Error
    }

    close(fd);
}

void setupForLoop(int bufferSize) {
    time(&startTime);
    buffer = (Message*) malloc(bufferSize * sizeof(Message));
    threads = (pthread_t*) malloc(sizeof(pthread_t));
    sem_init(&semaphore, 0, 1);
}

void exitLoop(int lastThread) {
    waitForAllThreads(lastThread);
    free(buffer);
    sem_destroy(&semaphore);
}

void waitForAllThreads(int lastThread) {
    for (int i = 0; i <= lastThread; ++i)
        pthread_join(threads[i], NULL);
    free(threads);
}

void registerOperation(int rid, int tskload, int pid, pthread_t tid,
    int tskres, char* oper) {
        printf("%lu ; %d ; %d ; %d ; %lu ; %d ; %s\n", time(NULL),
            rid, tskload, pid, tid, tskres, oper);
}
