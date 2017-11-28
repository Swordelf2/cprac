#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdlib.h>

volatile int flag = 0;

void
handler(int a)
{
    flag = 1;
}

void
work(int myid, int fd_rd, int fd_wr, int max_num, sigset_t set_prev)
{
    int opid;
    FILE *p_in = fdopen(fd_rd, "r");
    FILE *p_out = fdopen(fd_wr, "w");
    while (!flag) {
        sigsuspend(&set_prev);
    }
    flag = 0;

    fscanf(p_in, "%d", &opid);
    if (myid == 1) {
        fprintf(p_out, "%d ", getpid());
        fflush(p_out);
    } else if (myid == 2) {
        fprintf(p_out, "1 ");
        fflush(p_out);
    }

    kill(opid, SIGUSR1);

    while (1) {
        while (!flag) {
            sigsuspend(&set_prev);
        }
        flag = 0;
        int x;
        int sc_res;
        sc_res = fscanf(p_in, "%d", &x);
        if (sc_res != 1 || x == max_num) {
            fprintf(p_out, "%d ", max_num);
            fflush(p_out);
            break;
        }
        printf("%d %d\n", myid, x);
        fflush(stdout);
        fprintf(p_out, "%d ", x + 1);
        fflush(p_out);
        kill(opid, SIGUSR1);
    }
    fclose(p_in);
    fclose(p_out);
    kill(opid, SIGUSR1);
}

int
main(int argc, char *argv[])
{
    sigset_t set, set_prev;
    sigemptyset(&set);
    sigaddset(&set, SIGUSR1);
    sigprocmask(SIG_BLOCK, &set, &set_prev);
    sigaction(SIGUSR1, &(struct sigaction) {.sa_handler = handler, .sa_flags = SA_RESTART}, NULL);
    int fd[2];
    pipe(fd);
    pid_t ch1, ch2;

    int max_num = strtol(argv[1], NULL, 0);
    if ((ch1 = fork()) == 0) {
        work(1, fd[0], fd[1], max_num, set_prev);
        _exit(0);
    }
    if ((ch2 = fork()) == 0) {
        work(2, fd[0], fd[1], max_num, set_prev);
        _exit(0);
    }
    close(fd[0]);
    FILE *p_out = fdopen(fd[1], "w");
    fprintf(p_out, "%d ", ch2);
    fclose(p_out);
    kill(ch1, SIGUSR1);

    wait(NULL);
    wait(NULL);

    printf("Done\n");
    return 0;
}
