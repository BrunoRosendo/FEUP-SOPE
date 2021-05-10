#include "communication.h"

static time_t startTime;

static int numResults = 0;
static Message* buffer;
static pthread_t *threads;

static sem_t semaphore;
static Settings* settings;

void listenAndRespond(Settings* set) {
    settings = set;
    setupForLoop();
    int i = 0;

    while (1) {
        if (time(NULL) - startTime > settings->execTime) {
            fprintf(stderr, "[server] Execution time reached. Exiting...\n");
            break;
        }

        if (settings->fd == -1) {
            settings->fd = open(settings->fifoname, O_RDONLY, O_NONBLOCK);
            usleep(TIME_BETWEEN_ATTEMPTS_FIFO);
            continue;
        }

        getNewRequest(&i);
        dispatchResults();
    }
    exitLoop(i);
}

void *processRequest(void* arg) {
    while (1) {
        sem_wait(&semaphore);

        if (numResults >= settings->bufferSize) {
            sem_post(&semaphore);
            usleep(TIME_BETWEEN_ATTEMPTS_FIFO);
            continue;
        }

        Message* request = (Message*) arg;

        request->tskres = task(request->tskload);
        buffer[numResults++] = *request;

        free(request);
        break;
    }
    sem_post(&semaphore);
    return NULL;
}

void dispatchResults() {
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
        registerOperation(message.rid, message.tskload, message.pid,
                        message.tid, message.tskres, SERVER_SENT_RESULT);
    }

    numResults = 0;
    sem_post(&semaphore);
}

void getNewRequest(int* i) {
    Message* request = (Message*) malloc(sizeof(Message));

    if (read(settings->fd, request, sizeof(Message)) > 0) {
        // Success
        pthread_create(&threads[*i], NULL, processRequest, (void*)request);
        registerOperation(request->rid, request->tskload, getpid(),
                        pthread_self(), -1, SERVER_RCVD);

        ++(*i);
        threads = (pthread_t*) realloc(threads, ((*i) + 1) * sizeof(pthread_t));
    } else {
        // Error
    }
}

void setupForLoop() {
    time(&startTime);
    buffer = (Message*) malloc(settings->bufferSize * sizeof(Message));
    threads = (pthread_t*) malloc(sizeof(pthread_t));
    sem_init(&semaphore, 0, 1);
}

void exitLoop(int lastThread) {
    waitForAllThreads(lastThread);
    free(buffer);
    sem_destroy(&semaphore);
}

void waitForAllThreads(int lastThread) {
    for (int i = 0; i <= lastThread; ++i) {
        dispatchResults();
        pthread_join(threads[i], NULL);
    }
    free(threads);
}

void registerOperation(int rid, int tskload, int pid, pthread_t tid,
    int tskres, char* oper) {
        printf("%lu ; %d ; %d ; %d ; %lu ; %d ; %s\n", time(NULL),
            rid, tskload, pid, tid, tskres, oper);
}
