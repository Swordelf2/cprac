#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <linux/limits.h>
#include <string.h>
#include <stdlib.h>

void
start_execution(const char *file_name);

int
main(int argc, char *argv[])
{
    int N = strtol(argv[1], NULL, 0);
    for (int i = 2; i < N + 2 && i < argc; ++i) {
        start_execution(argv[i]);
    }
    unsigned good_count = 0;
    for (int i = 2; i < N + 2 && i < argc; ++i) {
        int status;
        wait(&status);
        if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
            ++good_count;
        }
    }
    for (int i = N + 2; i < argc; ++i) {
        start_execution(argv[i]);
        int status;
        wait(&status);
        if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
            ++good_count;
        }
    }
    printf("%u\n", good_count);
    return 0;
}


void
start_execution(const char *file_name)
{
    FILE *f = fopen(file_name, "r");
    char ex_name[PATH_MAX + 1];
    fgets(ex_name, sizeof(ex_name), f);
    fclose(f);
    size_t len = strlen(ex_name);
    if (ex_name[len - 1] == '\n') {
        ex_name[len - 1] = '\0';
    }
    if (fork() == 0) {
        execlp(ex_name, ex_name, NULL);
        _exit(1); // whoooops
    }
}
