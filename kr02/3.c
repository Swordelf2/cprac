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
    char *arg_arr[MAX_ARG_COUNT];
    for (int i = 1; i < argc; ++i) {
        char *prog_name = argv[i];
        ++i;
        arg_arr[0] = prog_name;
        int count = 1;
        while (argv[i] != NULL && strcmp(argv[i], "END") != 0) {
            arg_arr[count] = argv[i];
            ++i;
            ++count;
        }
        arg_arr[count] = NULL;
        
        if (fork() == 0) {
            execvp(prog_name, arg_arr);
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
