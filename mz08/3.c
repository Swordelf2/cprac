#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/wait.h>

enum
{
    MAX_STR_SIZE = 8,
    DEC_BASE = 10
};

int
main(void)
{
    setvbuf(stdin, NULL, _IONBF, 0);
    char str[MAX_STR_SIZE];
    for (int i = 1; i <= 3; ++i) {
        if (fork() == 0) {
            read(0, str, sizeof(str));
            str[sizeof(str) - 1] = '\0';
            int a = strtol(str, NULL, DEC_BASE);
            printf("%d %lld\n", i, (long long) a * a);
            exit(0);
        }
    }
    for (int i = 0; i < 3; ++i) {
        wait(NULL);
    }
    return 0;
}
