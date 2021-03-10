#include "aux.h"
#include "logs.h"
#include <sys/wait.h>

// assumes valid arguments
void changePermsWithOctal(const char *pathname, mode_t mode)
{
    chmod(pathname, mode);
}

<<<<<<< HEAD
void applyToPath(char *directoryPath, mode_t mode, Options *options) {
    if (options->recursive) {
        /*
        Opens a directory.
        Returns a valid pointer if the successfully, NULL otherwise
        */
=======
void applyToPath(char *directoryPath, mode_t mode, Options *options)
{
    if (options->recursive)
    {
        // opens a directory. Returns a valid pointer if the successfully, NULL otherwise
>>>>>>> Added recursive mode, using fork()
        DIR *dirPointer = opendir(directoryPath);

        struct dirent *dirEntry;
        struct stat inode;
        char name[1000];

<<<<<<< HEAD
        if (dirPointer == NULL) {
            // get info about the file/folder at the path name
            lstat(directoryPath, &inode);
            if (S_ISREG(inode.st_mode)) {  // if it is a file
=======
        if (dirPointer == NULL)
        {
            lstat(directoryPath, &inode); // get info about the file/folder at the path name
            if (S_ISREG(inode.st_mode))
            { // if it is a file
>>>>>>> Added recursive mode, using fork()
                changePermsWithOctal(directoryPath, mode);
            }
            else
            {
                fprintf(stderr, "Error opening directory\n");
            }
            return;
        }

<<<<<<< HEAD
        while ((dirEntry = readdir(dirPointer)) != 0) {
            /*
            sends formatted output to a string(name)
            name will be the absolute path to the next file
            */
            snprintf(name, sizeof(name), "%s/%s",
                    directoryPath, dirEntry->d_name);

            // get info about the file/folder at the path name
            lstat(name, &inode);
=======
        while ((dirEntry = readdir(dirPointer)) != 0)
        {
            // sends formatted output to a string(name) / name will be the absolute path to the next file
            sprintf(name, "%s/%s", directoryPath, dirEntry->d_name);
            lstat(name, &inode); //  get info about the file/folder at the path name
>>>>>>> Added recursive mode, using fork()

            // test the type of file
            if (S_ISDIR(inode.st_mode)){
                if (strcmp(dirEntry->d_name, ".") == 0 || strcmp(dirEntry->d_name, "..") == 0)
                    continue;

                int pid = fork();
                switch (pid){
                    case 0:
                        applyToPath(name, mode, options);
                        exit(0); // need to end the child process here, since otherwise it would print the files that are already being printed by the parent
                        break;
                    default:
                        printf("%s %s\n", "dir ", dirEntry->d_name);
                        int *stat_loc;
                        wait(stat_loc); // Waits for the child process to end
                        break;
                }
            }
            else if (S_ISREG(inode.st_mode))
            {
                printf("fis %s\n", dirEntry->d_name);
                changePermsWithOctal(name, mode);
            }
            else if (S_ISLNK(inode.st_mode))
            {
                printf("lnk %s\n", dirEntry->d_name);
            }
        }
        if (dirPointer != NULL)
            closedir(dirPointer);
    }
    else
    { // apply to the folder
        struct stat inode;
<<<<<<< HEAD
        // get info about the file/folder at the path name
        lstat(directoryPath, &inode);
        if (S_ISDIR(inode.st_mode) || S_ISREG(inode.st_mode)) {
=======
        lstat(directoryPath, &inode); // get info about the file/folder at the path name
        if (S_ISDIR(inode.st_mode) || S_ISREG(inode.st_mode))
        {
>>>>>>> Added recursive mode, using fork()
            changePermsWithOctal(directoryPath, mode);
        }
        else
        {
            fprintf(stderr, "You have not selected a valid path\n");
        }
    }
}

<<<<<<< HEAD
int main(int argc, char *argv[], char *envp[]) {
    if (argc < 3) {
        fprintf(stderr, "xmod: missing operand\n");
=======
int main(int argc, char *argv[], char *envp[])
{
    if (argc < 3)
    {
        fprintf(stderr, "Wrong number of arguments! ");
        printf("Call the function with:\nxmod [OPTIONS] MODE FILE/DIR\n");
>>>>>>> Added recursive mode, using fork()
        exit(1);
    }

    Options options;
    options.recursive = false; // default
    options.output = simple;

    for (int i = 1; i < argc - 2; ++i)
        parseFlag(argv[i], &options);

    char modeString[10];
    parseMode(argv[argc - 2], &options, modeString);
    mode_t mode;
    if (options.octal)
    {
        mode = getOctalFromOctalString(modeString);
    }
    else
    {
        mode = getOctalFromExplicitString(modeString, &options, argv[argc - 1]);
    }

    applyToPath(argv[argc - 1], mode, &options);

    return 0;
}
