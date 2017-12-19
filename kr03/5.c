#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

volatile int flag = 0;

void
handler(int sig)
{
    flag = 1;
}

int
main(int argc, char *argv[])
{
    sigset_t bset, dset;
    sigemptyset(&bset);
    sigaddset(&bset, SIGUSR1);
    sigprocmask(SIG_BLOCK, &bset, &dset);
    sigaction(SIGUSR1, &(struct sigaction) {.sa_handler = handler, .sa_flags = SA_RESTART}, NULL);
    
    int fd[2];
    pipe(fd);

    pid_t fpid = getpid(); // father pid
    pid_t son;
    if ((son = fork()) == 0) {
        pid_t gson;
        if ((gson = fork()) == 0) {
            // Gson
            long long mul = strtoll(argv[1], NULL, 0);
            while (1) {
                while (!flag) {
                    sigsuspend(&dset);
                }
                flag = 0;
                long long x;
                ssize_t count = read(fd[0], &x, sizeof(x));
                if (count == sizeof(int)) {
                    break;
                }
                printf("%lld ", x); fflush(stdout);
                // send to father
                x *= mul;
                write(fd[1], &x, sizeof(x));
                kill(fpid, SIGUSR1);
            }
            close(fd[1]);
            _exit(0);
        }
        // Son
        long long x;
        close(fd[0]);
        while (scanf("%lld", &x) == 1) {
            write(fd[1], &x, sizeof(x));
            kill(gson, SIGUSR1);
            while (!flag) {
                sigsuspend(&dset);
            }
            flag = 0;
        }
        int finish = 0;
        write(fd[1], &finish, sizeof(finish));
        kill(gson, SIGUSR1);
        close(fd[1]);
        wait(NULL);
        kill(fpid, SIGUSR1);
        _exit(0);
    }
    // Father
    close(fd[1]);
    while (1) {
        while (!flag) {
            sigsuspend(&dset);
        }
        flag = 0;
        long long x;
        if (read(fd[0], &x, sizeof(x)) == 0) {
            break;
        }
        printf("%lld ", x); fflush(stdout);
        kill(son, SIGUSR1);
    }

    wait(NULL);

    close(fd[0]);
    return 0;
}
