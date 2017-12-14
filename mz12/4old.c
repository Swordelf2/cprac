#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

#include <unistd.h>

void
process(int fd1_in, int fd2_in, int fd_out);

static inline int32_t
get_next(FILE *f, int *flag);

int
main(int argc, char *argv[])
{
    FILE *f[2];
    for (int i = 0; i < 2; ++i) {
        f[i] = fopen(argv[i + 1], "r");
    }

    int p[3][2];
    for (int i = 0; i < 3; ++i) {
        pipe(p[i]);
    }

    if (fork() == 0) {
        close(p[0][1]);
        close(p[1][1]);
        close(p[2][0]);
        process(p[0][0], p[1][0], p[2][1]);
        _exit(0);
    }
    close(p[0][0]);
    close(p[1][0]);
    close(p[2][1]);

    FILE *f1 = fdopen(p[0][1], "w");
    FILE *f2 = fdopen(p[1][1], "w");
    FILE *f_in = fdopen(p[2][0], "r");

    int32_t x;
    while (fscanf(f[0], "%"PRId32, &x) == 1) {
        fwrite(&x, sizeof(x), 1, f1);
    }
    fclose(f1);
    while (fscanf(f[1], "%"PRId32, &x) == 1) {
        fwrite(&x, sizeof(x), 1, f2);
    }
    fclose(f2);
    
    while (fread(&x, sizeof(x), 1, f_in) == 1) {
        printf("%"PRId32" ", x);
    }
    putchar('\n');

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
