#include <stdio.h>
#include <math.h>
#include <signal.h>

volatile int prime_nums[3];
volatile int count = 0;

enum
{
    DEC_BASE = 10
};

void
int_handler(int sig)
{
    // TODO: check count
    int cur_num = prime_nums[count++];
    printf("%d\n", cur_num); fflush(stdout);
}

void
term_handler(int sig)
{

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

// TODO: check if the arguments are in the command line !!
int
main(int argc, char *argv[])
{
    int low = strtol(argv[1], NULL, DEC_BASE);
    int high = strtol(argv[2], NULL, DEC_BASE);

    sigaction(SIGINT, &(struct sigaction) {.sa_handler = int_handler, .sa_flags = SA_RESTART}, NULL);
    sigaction(SIGINT, &(struct sigaction) {.sa_handler = term_handler, .sa_flags = SA_RESTART}, NULL);

    int calc_count = 0;

    for (int i = low; i < high && calc_count < 3; ++i) {
        if (isprime(i)) {
            prime_nums[calc_count++] = i;
        }
    }

    return 0;
}
