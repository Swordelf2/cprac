#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void
handler(int a)
{

}

int
main(void)
{
    sigaction(SIGINT, &(struct sigaction) {.sa_handler = handler}, NULL);
    sigset_t set, dfl_set;
    sigemptyset(&set);
    sigaddset(&set, SIGINT);
    sigprocmask(SIG_BLOCK, &set, &dfl_set);
    int a = 0;
    printf("%d\n", getpid());
    fflush(stdout);
    for (; a < 5; ++a) {
        sigsuspend(&dfl_set);
        if (a != 4) {
            printf("%d\n", a);
            fflush(stdout);
        }
    }
    return 0;
}
