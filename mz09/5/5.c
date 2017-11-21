#include <stdio.h>
#include <stdlib.h>

#include <linux/limits.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

extern char **environ;

enum
{
    MIN_ARG_COUNT = 2
};

int
main(int argc, char *argv[])
{
    if (argc < MIN_ARG_COUNT) {
        puts("0");
        exit(0);
    }

    const char *spt_path;
    if (!(spt_path = getenv("XDG_RUNTIME_DIR"))) {
        if (!(spt_path = getenv("TMPDIR"))) {
            spt_path = "/tmp";
        }
    }

    char spt_name[PATH_MAX];
    snprintf(spt_name, sizeof(spt_name), "%s/tmp_script%d", spt_path, getpid());
    
    int fd = creat(spt_name, 0700);
    FILE *spt = fdopen(fd, "w");
    fputs("#!/usr/bin/python3\n"
            "print(", spt);
    
    for (int i = 1; i < argc - 1; ++i) {
        fprintf(spt, "%s * ", argv[i]);
    }
    fprintf(spt, "%s)", argv[argc - 1]);
    fclose(spt);

    if (fork() == 0) {
        execve(spt_name, (char *[]) {spt_name, NULL}, environ);
    } 
    wait(NULL);
    unlink(spt_name);
    return 0;
}
