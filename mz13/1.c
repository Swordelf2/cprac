#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

enum
{
    NAME_SIZE = 128,
    BUF_SIZE = 1024
};

typedef struct Type
{
    size_t size;
    size_t align;
    char *name;
} Type;

char *
get_string(void);

int
Align(size_t *total_size, size_t align)
{
    if (*total_size & (align - 1)) {
        size_t temp = *total_size | (align - 1);
        if (__builtin_uaddl_overflow(1, temp, (unsigned long *) total_size)) {
            return 0;
        }
    }
    return 1;
}

int
main(void)
{
    char *str;
    size_t max_size = 16;
    size_t size = 0;
    Type *types = malloc(max_size * sizeof(*types));

    while (strcmp(get_string(), "END\n") != 0) {
        sscanf(str, "%s %zu %zu", types[size].name, &types[size].size, &types[size].align);
        ++size;
        if (size >= max_size) {
            types = realloc(types, (max_size *= 2) * sizeof(*types));
        }
    }
    
    size_t max_align = 0;
    size_t total_size = 0;
    int empty = 1;
    while (str = get_string()) {
        empty = 0;
        size_t count;
        sscanf(str, "%s %zu", name, &count);
        size_t c_size = 0, c_align = 0;
        for (size_t i = 0; i < size; ++i) {
            if (strcmp(name, types[i].name) == 0) {
                c_size = types[i].size;
                c_align = types[i].align;
                break;
            }
        }
        if (c_align > max_align) {
            max_align = c_align;
        }

        if (!Align(&total_size, c_align)) {
            goto ploho;
        }
        

        size_t temp;
        if (__builtin_umull_overflow(c_size, count, (unsigned long *) &temp)) {
            goto ploho;
        }
        if (__builtin_uaddl_overflow(total_size, temp, (unsigned long *) &total_size)) {
            goto ploho;
        }
    }
    
    if (empty) {
        printf("1 1\n");
        return 0;
    }

    if (!Align(&total_size, max_align)) {
        goto ploho;
    }
    

    printf("%zu %zu\n", total_size, max_align);
    return 0;
ploho:
    printf("0 0\n");
    return 0;
}

char *
get_string(void)
{
    static char *str = NULL;
    static size_t n = 0;
    ssize_t res = getline(&str, &n, stdin);
    return (res > 0) ? str : NULL;
}
