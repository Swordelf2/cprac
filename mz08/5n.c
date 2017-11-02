#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

int
main(void)
{
    int a;
    scanf("%d", &a);
    pid_t first_pid = fork();
    if (first_pid == 0) {
        do {
            pid_t pid = fork();
            if (pid == -1) {
                printf("-1\n");
                exit(-1);
            } else if (pid != 0) {
                int result;
                wait(&result);
                if (result == -1) {
                    exit(-1);
                } else {
                    printf("%d\n", a);
                    exit(0);
                }
            }
        } while (scanf("%d", &a) == 1);
    } else {
        scanf("%d", &a);
        int result;
        wait(&result);
        if (result != -1) {
            printf("%d\n", a);
        } 
    }
    return 0;
}
