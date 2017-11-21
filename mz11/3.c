#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <linux/limits.h>

void
start_execution(char *file_name);

int
main(int argc, char *argv[])
{
    unsigned N = strtoul(argv[1], NULL, 0);
    for (unsigned i = 2; i < N + 2; ++i) {
        start_execution(argv[i]);
    }
    unsigned good_count = 0;
    for (unsigned i = 2; i < N + 2; ++i) {
        int status;
        wait(&status);
        if (WIFEXITED(&status) && WEXITSTATUS(&status) == 0) {
            ++good_count;
        }
    }
    for (int i = N + 2; i < argc; ++i) {
        start_execution(argv[i]);
        wait(&status);
        if (WIFEXITED(&status) && WEXITSTATUS(&status) == 0) {
            ++good_count;
        }
}


void
start_execution(char *file_name)
{
    FILE *f = fopen(file_name, "r");
    char ex_name[PATH_MAX + 1];
    fgets(ex_name, sizeof(ex_name), stdin);
    if (fork() == 0) {
        execlp(ex_name, ex_name, NULL);
    }
}
