#include "client.h"

int init(char* argv[], Settings* settings) {
    if (parseCMDArgs(argv, settings))
        exit(1);

    srand(time(NULL));

    return syncWithServer(settings);
}

void exitProgram(Settings* settings) {
    close(settings->fd);
    unlink(settings->fifoname);
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Wrong number of arguments\n");
        printf("Call the program as c <-t nsecs> fifoname\n");
        exit(1);
    }

    Settings settings;
    if( init(argv, &settings) == 1){
        return 1;
    }
    generateRequests(&settings);
    exitProgram(&settings);
    return 0;
}
