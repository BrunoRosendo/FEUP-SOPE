#include "aux.h"

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Wrong number of arguments\n");
        printf("Call the program as c <-t nsecs> fifoname\n");
        exit(1);
    }

    Settings settings;
    init(argv, &settings);

    return 0;
}
