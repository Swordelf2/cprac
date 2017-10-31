#include <stdio.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/wait.h>

int
main(void)
{
    unsigned N;
    unsigned i = 1;
    printf("1");
    fflush(stdout);
    scanf("%u", &N);
    while (i < N) {
        if (fork() == 0) {
            ++i;
            printf(" %d", i);
            fflush(stdout);
        } else {
            break;
        }
    }
    wait(NULL);
    if (i == 1) {
        putchar('\n');
    }
    return 0;
}
