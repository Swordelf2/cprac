#include <stdio.h>
#include <signal.h>
#include <unistd.h>

int a;

enum
{
    COUNT = 5
};

void
handler(int sig)
{
    if (a != 4) {
        printf("%d\n", a);
        fflush(stdout);
    }
}

int
main(void)
{
    sigaction(SIGINT, &(struct sigaction) {.sa_handler = handler}, NULL);
    sigset_t set, dfl_set;
    sigemptyset(&set);
    sigaddset(&set, SIGINT);
    sigprocmask(SIG_BLOCK, &set, &dfl_set);
    printf("%d\n", getpid());
    fflush(stdout);
    for (a = 0; a < COUNT; ++a) {
        sigsuspend(&dfl_set);
    }
    return 0;
}
