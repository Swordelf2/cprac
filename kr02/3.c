#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <errno.h>

#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

enum
{
    MAX_ARG_COUNT = 1024
};

int
main(int argc, char *argv[])
{
    for (int i = 1; i < argc; ++i) {
        char *prog_name = argv[i];
        char **arg_start = argv + i;
        ++i;
        while (argv[i] != NULL && strcmp(argv[i], "END") != 0) {
            ++i;
        }
        argv[i] = NULL;
        
        if (fork() == 0) {
            execvp(prog_name, arg_start);
            _exit(1);
        }
        int status;
        wait(&status);
        if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
            exit(1);
        }
    }
    return 0;
}
