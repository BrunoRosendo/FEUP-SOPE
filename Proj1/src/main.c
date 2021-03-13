#include "aux.h"
#include "logs.h"

logInfo logInformation;

int main(int argc, char *argv[], char *envp[]) {
    subscribeSignals(argv[argc - 1]);
    setLogStart(&logInformation);

    if (argc < 3) {
        fprintf(stderr, "xmod: missing operand\n");
        exit(1);
    }

    setLogFile(envp, &logInformation);

    Options options;
    options.recursive = false;  // default
    options.output = simple;

    for (int i = 1; i < argc - 2; ++i)
        parseFlag(argv[i], &options);

    char modeString[10];
    char* clonedArgs[20];
    for (int i = 0; i < argc; i++) {
        clonedArgs[i] = (char *) malloc(100 * sizeof(char));
        memcpy(clonedArgs[i], argv[i], 100 * sizeof(char));
    }

    parseMode(argv[argc-2], &options, modeString);
    mode_t mode;
    if (options.octal) {
        mode = getOctalFromOctalString(modeString);
    } else {
        mode = getOctalFromExplicitString(modeString, &options, argv[argc - 1]);
    }

    applyToPath(argv[argc - 1], mode, &options, argc, clonedArgs);

    closeLogFile(&logInformation);

    while (1) {sleep(10);}
    return 0;
}
