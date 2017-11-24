#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

int
main(void)
{
    int a;
    int flag = 0;

    while (scanf("%d", &a) == 1) {
        pid_t pid = fork();
        if (pid == -1) {
            printf("-1\n");
            exit(-1);
        } else if (pid != 0) {
            int result;
            wait(&result);
            if (WEXITSTATUS(result) != 0) {
                if (flag == 0) {
                    exit(0);
                } else {
                    exit(-1);
                }
            } else {
                printf("%d\n", a);
                exit(0);
            }
        }
        flag = 1;
    }
}
