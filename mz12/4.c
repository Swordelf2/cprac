#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

struct Data
{
    pid_t pid;
    int id;
};

enum
{
    MAX_FILES = 1024,
};

void
process(FILE *f1, FILE *f2, FILE *f_out);

static inline int32_t
get_next(FILE *f, int *flag);

void
TextToBin(FILE *in, int out_fd);

int
main(int argc, char *argv[])
{
    int file_count = argc - 1;
    // tmp_names[i] is the name of the resulting file of the i-th process
    tmp_names[MAX_FILES];
    for (int i = 0; i < file_count; ++i) {
        // Create temporary files and write data into them
        tmp_names[i] = strdup(tmpnam(NULL));
        FILE *f_in = fopen(argv[i], "r");
        FILE *f_out = fopen(tmp_names[i], "w+");
        int32_t x;
        while (fscanf(f_in, "%"SCNd32, &x) == 1) {
            fwrite(&x, sizeof(x), 1, f_out);
        }
        fclose(f_in);
        fclose(f_out);
    }

    int fd[2];
    pipe(fd);

    int files_left = file_count;
   
    // Start all the initial processes
    for (int i = 0; i < file_count - 1; i += 2) {
        char *new_name = strdup(tmpnam(NULL));
        execute_work(tmp_names[i], tmp_names[i + 1], new_name);
        free(tmp_names[i]);
        free(tmp_names[i + 1]);
        tmp_names[i] = new_name;
    }

    // If there is still one file left
    if (file_count % 2 && file_count != 1) {
        struct Data data;
        read(fd[0], &data, sizeof(data));
        waitpid(data.pid, NULL, 0);
        char *new_name = strdup(tmpnam(NULL));
        execute_work(tmp_names[data.id], tmp_names[file_count - 1], data.id, fd);
    }

    while (files_left > 1) {
        struct Data data[2];
        // We wait until two processes are finished
        read(fd[0], &data[0], sizeof(data[0]));
        --files_left;
        if (files_left <= 1) {
            break;
        }
        read(fd[0], &data[1], sizeof(data[1]));
        --files_left;
        waitpid(data[0].pid, NULL, 0);
        waitpid(data[1].pid, NULL, 0);
        
        // Create new temp name and connect the resulting two files+
        // of the children we have waited for
        char *new_name = strdup(tmpnam(NULL));
        execute_work(tmp_names[data[0].id], tmp_names[data[1].id], data[0].id, fd[1]);
        free(tmp_names[data[0].id]);
        free(tmp_names[data[1].id]);
        tmp_names[data[0].id] = new_name;
    }
    
    return 0;
}

void
execute_work(const char *name1, const char *name2, int id, int p_out)
{
    if (fork() == 0) {
        FILE *f1 = fopen(name1, "r");
        FILE *f2 = fopen(name2, "r");
        FILE *f_out = fopen(name_out, "w+");
        process(f1, f2, f_out);
        fclose(f1);
        fclose(f2);
        fclose(f_out);
        struct Data data;
        data.pdi = getpid();
        data.id = id;
        write(p_out, &data, sizeof(data));
        _exit(0);
    }
}

void
process(FILE *f1, FILE *f2, FILE *f_out)
{
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
    fclose(out);
}
