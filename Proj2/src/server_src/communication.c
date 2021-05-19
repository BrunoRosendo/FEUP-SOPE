#include "communication.h"

static time_t startTime;

static int numResults = 0;
static Message* buffer;
static pthread_t *threads;

static sem_t semaphore;
static Settings* settings;

int serverTimeOver = 0;

void listenAndRespond(Settings* set) {
    settings = set;
    setupForLoop();
    int i = 0;

    while (1) {
        if (time(NULL) - startTime > settings->execTime) {
            fprintf(stderr, "[server] Execution time reached. Exiting...\n");
            serverTimeOver = 1;
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
    exitLoop(&i);
}

void *processRequest(void* arg) {
    /* Free the arg as soon as possible to avoid
    making the heap full */
    Message* toFree = (Message *)arg;
    Message request = *toFree;
    free(toFree);

    /* Should Calculate the task result here since
        if we do it inside the lock we would be wasting a lot of time
     */
    if (serverTimeOver) {
        request.tskres = -1;
    } else {
        request.tskres = task(request.tskload);
        registerOperation(request.rid, request.tskload, getpid(),
                pthread_self(), request.tskres, SERVER_PRODUCER_HAS_RESULT);
    }

    while (1) {
        sem_wait(&semaphore);

        if (numResults >= settings->bufferSize) {
            sem_post(&semaphore);
            usleep(TIME_BETWEEN_ATTEMPTS_FIFO);
            continue;
        }

        // if (serverTimeOver) request->tskres = -1;

        buffer[numResults++] = request;

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

        message.pid = getpid();
        message.tid = pthread_self();

        int fd = open(fifoName, O_WRONLY);

        if (fd == -1) {  // Error opening
            registerOperation(message.rid, message.tskload, message.pid,
                message.tid, -1, SERVER_PRIVATE_FIFO_CLOSED);
            continue;
        }

        if (write(fd, &message, sizeof(Message)) == -1) {
            registerOperation(message.rid, message.tskload, message.pid,
                message.tid, -1, SERVER_PRIVATE_FIFO_CLOSED);
            continue;
        }
        
        registerOperation(message.rid, message.tskload, message.pid,
            message.tid, message.tskres,
            message.tskres == -1? SERVER_REQUEST_2LATE : SERVER_SENT_RESULT);
        close(fd);
    }

    numResults = 0;
    sem_post(&semaphore);
}

int getNewRequest(int* i) {
    Message* request = (Message*) malloc(sizeof(Message));

    int readStatus = read(settings->fd, request, sizeof(Message));
    if (readStatus > 0) {
        // Success
        registerOperation(request->rid, request->tskload, getpid(),
                        pthread_self(), -1, SERVER_RCVD);
        pthread_create(&threads[*i], NULL, processRequest, (void*)request);

        ++(*i);
        
        threads = (pthread_t*) realloc(threads, ((*i) + 1) * sizeof(pthread_t));
    } else if (readStatus == 0) {
        return 0;
    }

    return 1;
}

void setupForLoop() {
    time(&startTime);
    buffer = (Message*) malloc(settings->bufferSize * sizeof(Message));
    threads = (pthread_t*) malloc(sizeof(pthread_t));
    sem_init(&semaphore, 0, 1);
}

void exitLoop(int *lastThread) {
    unlink(settings->fifoname);  // prevents the client to write to the fifo
    emptyPublicFifo(lastThread);
    waitForAllThreads(*lastThread);

    close(settings->fd);

    free(buffer);
    sem_destroy(&semaphore);
}

void waitForAllThreads(int lastThread) {
    for (int i = 0; i < lastThread; ++i) {
        pthread_join(threads[i], NULL);
        dispatchResults();  // call this after joining each thread
    }
    free(threads);
}

void emptyPublicFifo(int* i) {
    while (1) {
        int publicFifoStatus = getNewRequest(i);

        if (publicFifoStatus == 0) break;

        dispatchResults();
    }
}

void registerOperation(int rid, int tskload, int pid, pthread_t tid,
    int tskres, char* oper) {
        printf("%lu ; %d ; %d ; %d ; %lu ; %d ; %s\n", time(NULL),
            rid, tskload, pid, tid, tskres, oper);
}
