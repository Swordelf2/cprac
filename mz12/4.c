#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>

#include <unistd.h>

enum
{
    MAX_FILES = 1024,
    TEMPLATE_LEN = 1 + 3 + 1 + 3 + 6
};

const char template[] = "/tmp/huyXXXXXX";

void
process(int fd1_in, int fd2_in, int fd_out);

static inline int32_t
get_next(FILE *f, int *flag);

void
TextToBin(FILE *in, int out_fd);

int
main(int argc, char *argv[])
{
    FILE *f_in[MAX_FILES];
    int fd[MAX_FILES];
    for (int i = 0; i < argc - 1; ++i) {
        f_in[i] = fopen(argv[i + 1], "r");
    }
    char tmp_name[TEMPLATE_LEN + 1];
    int file_count = 0;
    for (int i = 0; i < argc - 1; ++i) {
        strcpy(tmp_name, template);
        fd[i] = mkstemp(
        TextToBin(f_in[i], fd[i]);
    
    return 0;
}

void
process(int fd1_in, int fd2_in, int fd_out)
{
    FILE *f1 = fdopen(fd1_in, "r");
    FILE *f2 = fdopen(fd2_in, "r");
    FILE *f_out = fdopen(fd_out, "w");

    int flag = 1;
    while (flag) {
        int32_t x, y;
        x = get_next(f1, &flag);
        y = get_next(f2, &flag);
        while (flag) {
            if (x == y) {
                fwrite(&x, sizeof(x), 1, f_out);
                x = get_next(f1, &flag);
                y = get_next(f2, &flag);
            } else if (x < y) {
                x = get_next(f1, &flag);
            } else {
                y = get_next(f2, &flag);
            }
        }
    }
    fclose(f1);
    fclose(f2);
    fclose(f_out);
}

static inline int32_t
get_next(FILE *f, int *flag)
{
    int32_t res;
    size_t ret = fread(&res, sizeof(res), 1, f) == 1;
    if (flag) {
        *flag = *flag && (ret == 1);
    }
    return res;
}

void
TextToBin(FILE *in, int out_fd)
{
    FILE *out = fdopen(out_fd, "w+");
    int32_t x;
    while (fscanf(in, "%"SCNd32, &x) == 1) {
        fwrite(&x, sizeof(x), 1, out);
    }
    fclose(
