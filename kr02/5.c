#include <stdio.h>
#include <stdlib.h>

#include <errno.h>

#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

int
start_exec(const char *cmd, int in_fd, int out_fd, int fd_toclose);

int
main(int argc, char *argv[])
{
    int status;
    int fd[2];
    pipe(fd);
    int pid = start_exec(argv[1], -1, fd[1], fd[0]);
    if (pid != -1) {
        wait(&status);
    }
    if (pid != -1 && WIFEXITED(status) && WEXITSTATUS(status) == 0) {
        start_exec(argv[2], -1, fd[1], fd[0]);
    }
    close(fd[1]);
    int f_fd = creat(argv[5], 0644);
    int pid3 = start_exec(argv[3], fd[0], f_fd, -1);
    waitpid(pid3, NULL, 0);
    start_exec(argv[4], fd[0], f_fd, -1);
    close(fd[0]);
    close(f_fd);
    while (wait(NULL) > 0);
    return 0;
}

int
start_exec(const char *cmd, int in_fd, int out_fd, int fd_toclose)
{
    pid_t pid = fork();
    if (pid == 0) {
        if (in_fd != -1) {
            dup2(in_fd, 0);

            close(in_fd);
        }
        if (out_fd != -1) {
            dup2(out_fd, 1);
            close(out_fd);
        }
        if (fd_toclose != -1) {
            close(fd_toclose);
        }
        execlp(cmd, cmd, NULL);
        _exit(1);
    }
    return pid;
}
