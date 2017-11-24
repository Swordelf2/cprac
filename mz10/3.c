#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

int
main(int argc, char *argv[])
{
    int fd[2];
    pipe(fd);
    if (fork() == 0) { // 1
        close(fd[0]);
        dup2(fd[1], 1);
        close(fd[1]);
        if (fork() == 0) { // 2
            int fd1 = open(argv[4], O_RDONLY);
            dup2(fd1, 0);
            close(fd1);
            execlp(argv[1], argv[1], NULL);
        }
        int status;
        wait(&status); // 2
        if (WEXITSTATUS(status) == 0) {
            if (fork() == 0) { // 3
                execlp(argv[2], argv[2], NULL);
            }
        }
        wait(NULL); // 3
    } else {
        close(fd[1]);
        if (fork() == 0) { // 4
            int fd2 = open(argv[5], O_CREAT | O_WRONLY | O_APPEND, 0644);
            dup2(fd[0], 0);
            close(fd[0]);
            dup2(fd2, 1);
            close(fd2);
            execlp(argv[3], argv[3], NULL);
        }
        close(fd[0]);
        wait(NULL); // 4
        wait(NULL); // 1
    }
    return 0;
}
