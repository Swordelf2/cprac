#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <string.h>

#include <linux/limits.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

extern char **environ;

enum
{
    MIN_ARG_COUNT = 2,
    MAX_TRY_COUNT = 64
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


    // Construct the script name based on pid and current time and try to create such file
    int fd;
    char spt_name[PATH_MAX];
    pid_t pid = getpid();
    for (int i = 0; i < MAX_TRY_COUNT; ++i) {
        unsigned rand_val = (unsigned) pid | ((unsigned) time(NULL) ^ (i + 1));
        snprintf(spt_name, sizeof(spt_name), "%s/tmp_script%u", spt_path, rand_val);
        
        fd = open(spt_name, O_RDWR | O_CREAT | O_EXCL, 0700);
        if (fd == -1) {
            if (errno != EEXIST) {
                fprintf(stderr, "Error while trying to create script: %s\n", strerror(errno));
                exit(1);
            }
        } else {
            break;
        }
    }
    if (fd == -1) {
        fputs("Could not create script\n", stderr);
        exit(1);
    }
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
