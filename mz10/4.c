#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

struct Array
{
    pid_t *arr;
    size_t size;
    size_t max_size;
};

enum ArrayConstants
{
    ARR_INIT_SIZE = 16,
    ARR_EXTEND_MUL = 2
};

int
array_init(struct Array *array);

int
array_extend(struct Array *array);

int
array_append(struct Array *array, pid_t new_pid);

void
array_kill_all(const struct Array *array);

void
array_destroy(struct Array *array);

pid_t
start_execution(const char *cmd, int fd_in, int fd_out, int fd_to_close);
// fd_to_close is the fd which should be closed in the child
// returns -1 on error, and new pid of created child on success

int
main(int argc, char *argv[])
{
    int fail_flag = 0;
    struct Array pid_array;
    if (!array_init(&pid_array)) {
        fail_flag = 1;
        goto main_exit;
    }
    int p[2], prev_p[2];
    int fd_in_toclose = -1; // 
    // p is the right pipe, prev_p is the left pipe of the current command
    // fd_in_toclose is read end of the pipe to the left of the previous command
    for (int i = 1; i < argc; ++i) {
        if (fd_in_toclose != -1) {
            close(fd_in_toclose);
        }
        if (i == 1) {
            prev_p[0] = 0;
        } else {
            close(prev_p[1]);
            fd_in_toclose = prev_p[0];
        }
        if (i == argc - 1) {
            p[1] = 1;
            p[0] = -1; // so that start_execution doesn't close stdout
        } else {
            if (pipe(p) == -1) {
                fail_flag = 1;
                goto main_exit;
            }
        }
        pid_t ch_pid = start_execution(argv[i], prev_p[0], p[1], p[0]);
        if (ch_pid == -1) {
            fail_flag = 1;
            goto main_exit;
        } else {
            array_append(&pid_array, ch_pid);
        }
        memcpy(prev_p, p, sizeof(prev_p));
    }
    if (fd_in_toclose != -1) {
        close(fd_in_toclose);
    }

main_exit:
    if (fail_flag) {
        array_kill_all(&pid_array);
    }
    // Wait for all
    while (wait(NULL) > 0);
    array_destroy(&pid_array);
    return fail_flag;
}

pid_t
start_execution(const char *cmd, int fd_in, int fd_out, int fd_to_close)
{
    pid_t pid = fork();
    if (pid == -1) {
        return -1;
    } else if (pid == 0) {
        if (fd_to_close != -1) {
            close(fd_to_close);
        }
        if (fd_in != 0) {
            dup2(fd_in, 0);
            close(fd_in);
        }
        if (fd_out != 1) {
            dup2(fd_out, 1);
            close(fd_out);
        }
        execlp(cmd, cmd, NULL);
        _exit(1);
    }
    return pid;
}

int
array_init(struct Array *array)
{
    array->max_size = ARR_INIT_SIZE;
    if (!(array->arr = malloc(array->max_size * sizeof(*array->arr)))) {
        return 0;
    }
    array->size = 0;
    return 1;
}

int
array_extend(struct Array *array)
{
    array->max_size *= ARR_EXTEND_MUL;
    pid_t *new_ptr = realloc(array->arr, array->max_size * sizeof(*array->arr));
    if (!new_ptr) {
        return 0;
    }
    array->arr = new_ptr;
    return 1;
}

int
array_append(struct Array *array, pid_t new_pid)
{
    if (array->size >= array->max_size) {
        if (!array_extend(array)) {
            return 0;
        }
    }
    array->arr[array->size++] = new_pid;
    return 1;
}

void
array_kill_all(const struct Array *array)
{
    for (size_t i = 0; i < array->size; ++i) {
        kill(array->arr[i], SIGKILL);
    }
}

void
array_destroy(struct Array *array)
{
    free(array->arr);
}
