#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

int
main(void)
{
    long long a;
    pid_t first_pid = fork();
    if (first_pid == 0) {
        while (scanf("%lld", &a) == 1) {
            pid_t pid = fork();
            if (pid == -1) {
                printf("-1\n");
                exit(-1);
            } else if (pid != 0) {
                int result;
                wait(&result);
                if (WEXITSTATUS(result) != 0) {
                    exit(-1);
                } else {
                    printf("%lld\n", a);
                    exit(0);
                }
            }
        }
    }
    wait(NULL);
    return 0;
}
