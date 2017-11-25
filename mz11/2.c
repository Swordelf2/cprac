#include <stdio.h>
#include <signal.h>
#include <unistd.h>

enum handle_type
{
    REVERT,
    SQR
};

volatile int h_type = REVERT;

void
handler1(int a)
{
    h_type = REVERT;
}

void
handler2(int a)
{
    h_type = SQR;
}

int
main(void)
{
    sigaction(SIGUSR1, 
            &(struct sigaction) {.sa_handler = handler1, .sa_flags = SA_RESTART}, 
            NULL);
    sigaction(SIGUSR2, 
            &(struct sigaction) {.sa_handler = handler2, .sa_flags = SA_RESTART}, 
            NULL);
    printf("%d\n", getpid());
    fflush(stdout);
    long long a;
    while (scanf("%lld", &a) == 1) {
        if (h_type == REVERT) {
            printf("%lld\n", -a);
        } else {
            printf("%lld\n", a * a);
        }
        fflush(stdout);
    }
    return 0;
}
