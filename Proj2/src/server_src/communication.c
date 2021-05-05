#include "communication.h"

void syncWithClient(Settings* settings) {
    fprintf(stderr, "[server] Synchronizing with client...\n");

    

    fprintf(stderr, "[server] Client synchronized with success\n");
}

void registerOperation(int rid, int tskload, int pid, pthread_t tid,
    int tskres, char* oper) {
        printf("%lu ; %d ; %d ; %d ; %lu ; %d ; %s\n", time(NULL),
            rid, tskload, pid, tid, tskres, oper);
}
