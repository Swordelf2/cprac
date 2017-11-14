#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>

int
main(void)
{
    int fd[2];
    pipe(fd);
    if (fork() == 0) {
        close(fd[1]);
        if (fork() == 0) {
            int a[2];
            long long sum = 0;
            while (1) {
                read(fd[0], a, sizeof(a));
                if (a[1] == -1) {
                    break;
                }
                sum += a[0];
            }
            close(fd[0]);
            printf("%lld\n", sum);
        } else {
            close(fd[1]);
            wait(NULL);
        }
    } else {
        int num;
        int b[2];
        while (scanf("%d", &num) == 1) {
            b[0] = num;
            b[1] = 0;
            write(fd[1], b, sizeof(b));
        }
        b[1] = -1;
        write(fd[1], b, sizeof(b));
        wait(NULL);
    }
    return 0;
}
