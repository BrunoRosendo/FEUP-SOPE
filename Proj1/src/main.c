#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include "parsing.h"
#define LOGFILE "LOG_FILENAME"

bool setLogFile(char **envp, FILE *file)
{
    for (char **env = envp; *env != 0; env++)
    {
        char *thisEnv = *env;
        if (strstr(thisEnv, LOGFILE) != 0)
        {
            const char delim[2] = "=";
            strtok(thisEnv, delim);
            char *filename = strtok(NULL, delim);
            file = fopen(filename, "w");
            return true;
        }
    }
    return false;
}

// assumes valid arguments
void changePermsWithOctal(const char *pathname, mode_t mode){
    chmod(pathname, mode);
}

void applyToDirectory(char* directoryPath, mode_t mode){
    // opens a directory. Returns a valid pointer if the successfully, NULL otherwise
    DIR *dirPointer = opendir(directoryPath);

    struct dirent *dirEntry;
    struct stat inode;
    char name[1000];

    if (dirPointer == NULL){
        lstat(directoryPath, &inode);                                     // get info about the file/folder at the path name
        if(S_ISREG(inode.st_mode)){     // if it is a file
            changePermsWithOctal(directoryPath, mode);
        }
        else{
            printf("Error opening directory\n");
        }
        return;
    }

    while ((dirEntry = readdir(dirPointer)) != 0)
    {
        sprintf(name, "%s/%s", directoryPath, dirEntry->d_name); // sends formatted output to a string(name) / name will be the absolute path to the next file
        lstat(name, &inode);                                     // get info about the file/folder at the path name

        // test the type of file
        if (S_ISDIR(inode.st_mode))
            printf("dir ");
        else if (S_ISREG(inode.st_mode)){
            printf("fis ");
            changePermsWithOctal(name, mode);
        }
        else if (S_ISLNK(inode.st_mode))
            printf("lnk ");
        else
            ;
        printf(" %s\n", dirEntry->d_name);
    }
}

int main(int argc, char* argv[], char* envp[]) {
    if (argc < 3) {
        fprintf(stderr, "Wrong number of arguments! ");
        printf("Call the function with:\nxmod [OPTIONS] MODE FILE/DIR\n");
        exit(1);
    }

    Options options;
    char modeString[10];
    parseMode(argv[argc - 2], &options, modeString);
    mode_t mode;
    if (options.octal) {
        mode = getOctalFromString(modeString);
    } else {
        mode = getOctalFromDecimalString(argv[argc - 1], modeString, &options);
    }

    applyToDirectory(argv[argc-1], mode);
    
    return 0;
}

