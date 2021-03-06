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
main(int argc, char *argv[])
{
    if (fork() == 0) {
        int fd = open(argv[2], O_RDONLY);
        dup2(fd, 0);
        close(fd);
        fd = creat(argv[3], 0666);
        dup2(fd, 1);
        close(fd);
        execlp(argv[1], argv[1], NULL);
        _exit(1);
    } else {
        wait(NULL);
    }
    return 0;
}
