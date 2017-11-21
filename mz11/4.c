#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdlib.h>

int opid;
volatile int flag;

void
handler(int a)
{
    flag = 1;
}

void
work(int myid, int fd_rd, int fd_wr, int max_num)
{
    sigset_t set, set_prev;
    sigemptyset(&set);
    sigaddset(&set, SIGUSR1);
    sigprocmask(SIG_BLOCK, &set, &set_prev);
    sigaction(SIGUSR1, &(struct sigaction) {.sa_handler = handler}, NULL);

    FILE *p_in = fdopen(fd_rd, "r");
    FILE *p_out = fdopen(fd_wr, "w");

    while (!flag) {
        sigsuspend(&set_prev);
    }

    fscanf(p_in, "%d ", &opid);
    kill(opid, SIGUSR1);

    while (1) {
        while (!flag) {
            sigsuspend(&set_prev);
        }
        flag = 0;
        int x;
        int sc_res;
        sc_res = fscanf(p_in, "%d ", &x);
        if (sc_res != 1 || x == max_num) {
            break;
        }
        printf("%d %d\n", myid, x);
        fflush(stdout);
        fprintf(p_out, "%d ", x + 1);
        fflush(p_out);
    }
    fclose(p_in);
    fclose(p_out);
    kill(opid, SIGUSR1);
}

int
main(int argc, char *argv[])
{
    int fd[2];
    pipe(fd);
    pid_t ch1, ch2;

    if ((ch1 = fork()) == 0) {
        work(1, fd[0], fd[1], strtol(argv[0], NULL, 0));
        exit(0);
    }
    if ((ch2 = fork()) == 0) {
        work(2, fd[0], fd[1], strtol(argv[0], NULL, 0));
        exit(0);
    }
    close(fd[0]);
    FILE *p_out = fdopen(fd[1], "w");
    fprintf(p_out, "%d %d ", ch2, ch1);
    fclose(p_out);
    kill(ch1, SIGUSR1);
    wait(NULL);
    wait(NULL);
    printf("Done\n");
}

