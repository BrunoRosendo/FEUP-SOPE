#include "aux.h"
#include "logs.h"

// assumes valid arguments
void changePermsWithOctal(const char *pathname, mode_t mode) {
    chmod(pathname, mode);
}

void applyToPath(char *directoryPath, mode_t mode, Options *options) {
    if (options->recursive) {
        /*
        Opens a directory.
        Returns a valid pointer if the successfully, NULL otherwise
        */
        DIR *dirPointer = opendir(directoryPath);

        struct dirent *dirEntry;
        struct stat inode;
        char name[1000];

        if (dirPointer == NULL) {
            // get info about the file/folder at the path name
            lstat(directoryPath, &inode);
            if (S_ISREG(inode.st_mode)) {  // if it is a file
                changePermsWithOctal(directoryPath, mode);
            } else {
                fprintf(stderr, "Error opening directory\n");
            }
            return;
        }

        while ((dirEntry = readdir(dirPointer)) != 0) {
            /*
            sends formatted output to a string(name)
            name will be the absolute path to the next file
            */
            snprintf(name, sizeof(name), "%s/%s",
                    directoryPath, dirEntry->d_name);

            // get info about the file/folder at the path name
            lstat(name, &inode);

            // test the type of file
            if (S_ISDIR(inode.st_mode)) {
                printf("dir ");
            } else if (S_ISREG(inode.st_mode)) {
                printf("fis ");
                changePermsWithOctal(name, mode);
            } else if (S_ISLNK(inode.st_mode)) {
                printf("lnk ");
            }
            printf(" %s\n", dirEntry->d_name);
        }
        if (dirPointer != NULL) closedir(dirPointer);
    } else {   // apply to the folder
        struct stat inode;
        // get info about the file/folder at the path name
        lstat(directoryPath, &inode);
        if (S_ISDIR(inode.st_mode) || S_ISREG(inode.st_mode)) {
            changePermsWithOctal(directoryPath, mode);
        } else {
            fprintf(stderr, "You have not selected a valid path\n");
        }
    }
}

int main(int argc, char *argv[], char *envp[]) {
    if (argc < 3) {
        fprintf(stderr, "Wrong number of arguments! ");
        printf("Call the function with:\nxmod [OPTIONS] MODE FILE/DIR\n");
        exit(1);
    }

    Options options;
    options.recursive = false;  // default
    options.output = simple;

    for (int i = 1; i < argc - 2; ++i)
        parseFlag(argv[i], &options);

    char modeString[10];
    parseMode(argv[argc - 2], &options, modeString);
    mode_t mode;
    if (options.octal) {
        mode = getOctalFromOctalString(modeString);
    } else {
        mode = getOctalFromExplicitString(modeString, &options, argv[argc-1]);
    }

    applyToPath(argv[argc - 1], mode, &options);

    return 0;
}
