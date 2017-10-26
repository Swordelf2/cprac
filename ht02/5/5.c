#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct Stack
{
    char **arr;
    size_t size;
    size_t max_size;
} Stack;

enum 
{
    STCK_INIT_SIZE = 16,
    STCK_EXTEND_MUL = 2
};


// All these Stack functions return 0 on failure, non-zero on success
int
stack_init(Stack *stack);

int
stack_extend(Stack *stack);

int
stack_push(Stack *stack, char *name);

char *
stack_pop(Stack *stack);

int
stack_isempty(Stack *stack);

static void
safe_exit(DIR *saved_cur_dir, Stack *stack_to_free);

ssize_t
getcwd2(int fd, char *buf, size_t size)
{
    DIR *saved_cur_dir = opendir(".");
    if (!saved_cur_dir) {
        return -1;
    }

    ino_t root_ino;
    dev_t root_dev;
    struct stat buf_stat;
    if (lstat("/", &buf_stat) != 0) {
        safe_exit(saved_cur_dir, NULL);
        return -1;
    }
    root_ino = buf_stat.st_ino;
    root_dev = buf_stat.st_dev;

    if (fchdir(fd) != 0) {
        safe_exit(saved_cur_dir, NULL);
        return -1;
    }

    Stack stack;
    if (!stack_init(&stack)) {
        safe_exit(saved_cur_dir, NULL);
        return -1;
    }
    ino_t prev_ino;
    dev_t prev_dev;
    while (1) {
        if (lstat(".", &buf_stat) != 0) {
            safe_exit(saved_cur_dir, &stack);
            return -1;
        }
        if (buf_stat.st_ino == root_ino && buf_stat.st_dev == root_dev) {
            break;
        }

        prev_ino = buf_stat.st_ino;
        prev_dev = buf_stat.st_dev;

        if (chdir("..") != 0) {
            safe_exit(saved_cur_dir, &stack);
            return -1;
        }

        DIR *cur_dir = opendir(".");
        if (!cur_dir) {
            safe_exit(saved_cur_dir, &stack);
            return -1;
        }
        struct dirent *dir_ent;
        int found_dir_flag = 0;
        while ((dir_ent = readdir(cur_dir))) {
            if (lstat(dir_ent->d_name, &buf_stat) != 0) {
                closedir(cur_dir);
                safe_exit(saved_cur_dir, &stack);
                return -1;
            }
            if (buf_stat.st_ino == prev_ino && buf_stat.st_dev == prev_dev) {
                if (!stack_push(&stack, dir_ent->d_name)) {
                    closedir(cur_dir);
                    safe_exit(saved_cur_dir, &stack);
                    return -1;
                }
                found_dir_flag = 1;
            }
        }
        closedir(cur_dir);
        if (!found_dir_flag) {
            safe_exit(saved_cur_dir, &stack);
            return -1;
        }
    }

    if (stack_isempty(&stack)) {
        int pr_ret;
        pr_ret = snprintf(buf, size, "/");
        safe_exit(saved_cur_dir, &stack);
        if (pr_ret < 0) {
            return -1;
        }
        return 1;
    }
    size_t full_path_len = 0;
    while (!stack_isempty(&stack)) {
        char *cur_name = stack_pop(&stack);
        int pr_ret;
        if (full_path_len < size) {
            pr_ret = snprintf(buf + full_path_len,
                    size - full_path_len,
                    "/%s",
                    cur_name);
            if (pr_ret < 0) {
                safe_exit(saved_cur_dir, &stack);
                return -1;
            }
        }
        full_path_len += strlen(cur_name) + 1;
        free(cur_name);
    }
    safe_exit(saved_cur_dir, &stack);
    return full_path_len;
}

int
stack_init(Stack *stack)
{
    stack->arr = malloc((stack->max_size = STCK_INIT_SIZE) * sizeof(stack->arr[0]));
    if (!stack->arr) {
        return 0;
    }
    stack->size = 0;
    return 1;
}

int
stack_extend(Stack *stack)
{
    stack->max_size *= STCK_EXTEND_MUL;
    char **new_ptr = realloc(stack->arr, stack->max_size * sizeof(stack->arr[0]));
    if (!new_ptr) {
        return 0;
    }
    stack->arr = new_ptr;
    return 1;
}

int
stack_push(Stack *stack, char *name)
{
    if (stack->size >= stack->max_size) {
        if (!stack_extend(stack)) {
            return 0;
        }
    }
    stack->arr[stack->size++] = strdup(name);
    return 1;
}

char *
stack_pop(Stack *stack)
{
    if (stack->size == 0) {
        return NULL;
    }
    return stack->arr[--stack->size];
}

void
stack_free(Stack *stack)
{
    for (size_t i = 0; i < stack->size; ++i) {
        free(stack->arr[i]);
    }
    free(stack->arr);
}

int
stack_isempty(Stack *stack)
{
    return stack->size == 0;
}

static void
safe_exit(DIR *saved_cur_dir, Stack *stack_to_free)
{
    fchdir(dirfd(saved_cur_dir));
    closedir(saved_cur_dir);
    if (stack_to_free) {
        stack_free(stack_to_free);
    }
}
