#include "aux.h"
#include "logs.h"

int main(int argc, char *argv[], char *envp[]) {
    setLogStart();  // Start the timer
    subscribeSignals(argv[argc - 1]);

    if (argc < 3) {
        fprintf(stderr, "xmod: missing operand\n");
        logExit(1);
        exit(1);
    }

    setLogFile(argc, argv);  // Start logging
    logProcessCreation(argc, argv);
    Options options;
    options.recursive = false;  // default
    options.output = simple;

    for (int i = 1; i < argc - 2; ++i)
        parseFlag(argv[i], &options);

    char modeString[10];
    char* clonedArgs[20];
    for (int i = 0; i < argc; i++) {
        clonedArgs[i] = (char *) malloc(100 * sizeof(char));
        snprintf(clonedArgs[i], 100 * sizeof(char), "%s", argv[i]);
    }
    clonedArgs[argc] = NULL;    // Array needs to end with NULL

    parseMode(argv[argc-2], &options, modeString);
    mode_t mode;
    if (options.octal) {
        mode = getOctalFromOctalString(modeString);
    } else {
        mode = getOctalFromExplicitString(modeString, &options, argv[argc - 1]);
    }

    applyToPath(argv[argc - 1], mode, &options, argc, clonedArgs);

    //for (int i = 0; i < argc; i++)
      //  free(clonedArgs[i]);

    logExit(0);  // Register logging exit from the process
    closeLogFile();
    return 0;
}
