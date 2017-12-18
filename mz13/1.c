#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

enum
{
    INIT_MAX_SIZE = 16,
    EXTEND_MUL = 2
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
Align(size_t *total_size, size_t align);

void
free_array(Type *array, size_t size);

int
main(void)
{
    size_t max_size = 16;
    size_t size = 0;
    Type *types = malloc(max_size * sizeof(*types));

    while (1) {
        // Read name
        char *name = get_string();
        if (strcmp(name, "END") == 0) {
            break;
        }
        // Extend the array of types if needed
        if (size >= max_size) {
            types = realloc(types, (max_size *= EXTEND_MUL) * sizeof(*types));
        }
        types[size].name = name;
        // Now read other fields of the struct Type
        scanf("%zu %zu", &types[size].size, &types[size].align);
        getchar(); // Read the trailing '\n'
        ++size;
    }
    
    size_t max_align = 0;
    size_t total_size = 0;
    int empty = 1;
    char *str;
    while ((str = get_string())) {
        empty = 0;
        size_t count;
        char *name = str;
        // Now search for this name in the array of types
        size_t c_size = 0, c_align = 0;
        for (size_t i = 0; i < size; ++i) {
            if (strcmp(name, types[i].name) == 0) {
                c_size = types[i].size;
                c_align = types[i].align;
                break;
            }
        }
        free(name);
        // Read the count now
        scanf("%zu", &count);
        getchar(); // Trailing '\n'
        
        if (c_align > max_align) {
            max_align = c_align;
        }

        // Preliminary alignment
        if (!Align(&total_size, c_align)) {
            goto fail_label;
        }
        
        size_t temp;
        if (__builtin_umull_overflow(c_size, count, (unsigned long *) &temp)) {
            goto fail_label;
        }
        if (__builtin_uaddl_overflow(total_size, temp, (unsigned long *) &total_size)) {
            goto fail_label;
        }
    }
    
    if (empty) {
        printf("1 1\n");
        return 0;
    }

    if (!Align(&total_size, max_align)) {
        goto fail_label;
    }
    

    printf("%zu %zu\n", total_size, max_align);
    free_array(types, size);
    return 0;
fail_label:
    printf("0 0\n");
    free_array(types, size);
    return 0;
}

char *
get_string(void)
{
    size_t size = 0;
    size_t max_size = INIT_MAX_SIZE;
    char *str = malloc(max_size);
    int c;
    while (1) {
        c = getchar_unlocked();
        if (c == EOF && size == 0) {
            free(str);
            return NULL;
        }
        if (c == ' ' || c == '\n' || c == EOF) {
            break;
        } else {
            // Extend the string
            if (size + 1 >= max_size) {
                str = realloc(str, max_size *= EXTEND_MUL);
            }
            str[size++] = c;
        }
    }
    str[size] = '\0';
    return str;
}

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

void
free_array(Type *array, size_t size)
{
    for (size_t i = 0; i < size; ++i) {
        free(array[i].name);
    }
    free(array);
}
