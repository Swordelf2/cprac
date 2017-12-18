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


    // Construct the script name based on values from /dev/random
    int rand_fd = open("/dev/random", O_RDONLY);
    int fd;
    char spt_name[PATH_MAX];
    for (int i = 0; i < MAX_TRY_COUNT; ++i) {
        unsigned long long rand_val;
        read(rand_fd, &rand_val, sizeof(rand_val));
        snprintf(spt_name, sizeof(spt_name), "%s/tmp_script%llu", spt_path, rand_val);
        
        fd = open(spt_name, O_WRONLY | O_CREAT | O_EXCL, 0700);
        if (fd == -1) {
            if (errno != EEXIST) {
                fprintf(stderr, "Error while trying to create script: %s\n", strerror(errno));
                exit(1);
            }
        } else {
            break;
        }
    }
    close(rand_fd);

    if (fd == -1) {
        fputs("Could not create script\n", stderr);
        exit(1);
    }
    FILE *spt = fdopen(fd, "w");
    fputs("#!/usr/bin/python3\n"
            "import os\n"
            "print(", spt);
    
    for (int i = 1; i < argc - 1; ++i) {
        fprintf(spt, "%s * ", argv[i]);
    }
    fprintf(spt, "%s)\n", argv[argc - 1]);
    fprintf(spt, "os.remove(\"%s\")", spt_name);
    fclose(spt);
    execve(spt_name, (char *[]) {spt_name, NULL}, environ);
    fprintf(stderr, "Could not execute script: %s", strerror(errno));
    return 1;
}
