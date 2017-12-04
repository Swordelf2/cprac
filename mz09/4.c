#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>

int
exec_command(char *command)
{
    pid_t pid;
    pid = fork();
    if (pid == 0) {
        execlp(command, command, NULL);
        _exit(1);
    } else if (pid == -1) {
        return 1;
    } else {
        int status;
        waitpid(pid, &status, 0);
        return WIFEXITED(status) && (WEXITSTATUS(status) == 0);
        if (!WIFEXITED(status)) {
            return 1;
        }
        return WEXITSTATUS(status);
    }
}

int
main(int argc, char *argv[])
{
    return !((exec_command(argv[1]) ||
            exec_command(argv[2])) &&
            exec_command(argv[3]));
}
