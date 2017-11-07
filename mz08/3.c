#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/wait.h>

enum
{
    MAX_STR_SIZE = 8 + 1
};

int
main(void)
{
    setvbuf(stdin, NULL, _IOFBF, 0);
    char str[MAX_STR_SIZE];
    for (int i = 1; i <= 3; ++i) {
        if (fork() == 0) {
            fgets(str, sizeof(str), stdin);
            int a = strtol(str, NULL, 10);
            printf("%d %lld\n", i, (long long) a * a);
            exit(0);
        }
    }
    for (int i = 0; i < 3; ++i) {
        wait(NULL);
    }
    return 0;
}