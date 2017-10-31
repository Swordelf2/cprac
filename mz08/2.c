#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include <sys/types.h>
#include <sys/wait.h>

int
main(void)
{
    if (fork() == 0) {
        if (fork() == 0) {
            printf("3 ");
            exit(0);
        } else {
            wait(NULL);
        }
        printf("2 ");
        exit(0);
    } else {
        wait(NULL);
        printf("1\n");
    }
    return 0;
}
