#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <math.h>
#include <sys/types.h>
#include <unistd.h>


volatile int last_prime = 0;


enum
{
    DEC_BASE = 10,
    ARG_POS_LOW = 1,
    ARG_POS_HIGH = 2
};

void
int_handler(int sig)
{
    static int count = 0;
    if (count >= 3) {
        _exit(0);
    }
    printf("%d\n", last_prime);
    fflush(stdout);
    ++count;
}

void
term_handler(int sig)
{
    _exit(0);
}

int
isprime(int a)
{
    int sqr_rt = sqrt(a);
    for (int i = 2; i <= sqr_rt; ++i) {
        if (a % i == 0) {
            return 0;
        }
    }
    return 1;
}

int
main(void)
{
    int low, high;
    scanf("%d %d", &low, &high);

    sigaction(SIGINT, &(struct sigaction) {.sa_handler = int_handler}, NULL);
    sigaction(SIGTERM, &(struct sigaction) {.sa_handler = term_handler}, NULL);

    printf("%d\n", getpid()); fflush(stdout);

    for (int i = low; i < high; ++i) {
        if (isprime(i)) {
            last_prime = i;
        }
    }

    // Block signals before printing
    sigset_t block_set;
    sigemptyset(&block_set);
    sigaddset(&block_set, SIGINT);
    sigaddset(&block_set, SIGTERM);
    sigprocmask(SIG_BLOCK, &block_set, NULL);

    printf("-1\n"); fflush(stdout);
    return 0;
}
