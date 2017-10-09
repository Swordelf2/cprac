#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum
{
    ARR_EXTEND_MULT = 2, // resizable array extension multiplier
    ARR_INIT_SIZE = 1 // TODO: change this value!
};

// Resizable array of strings
typedef struct Array
{
    char **arr;
    size_t size;
    size_t max_size;
} Array;


int
array_init(Array *array);

int
array_extend(Array *array);

int
array_add(Array *array, char *new_str);
// COPIES string new_str into array

void
array_delete(Array *array);

int
main(void)
{
    Array my_arr;
    array_init(&my_arr);
    char buff[1024];
    while (fgets(buff, sizeof(buff), stdin)) {
        array_add(&my_arr, buff);
    }
    
    // TODO: use NAME_MAX
    // USE lstat instead of stat
    int n;
    while (scanf("%d", &n) == 1) {
        fputs(my_arr.arr[n], stdout);
    }
    
    return 0;
}

int
array_init(Array *array)
{
    array->arr = malloc(ARR_INIT_SIZE * sizeof(array->arr[0]));
    if (!array->arr) {
        return 0;
    }
    array->size = 0;
    array->max_size = ARR_INIT_SIZE;
    return 1;
}

int
array_extend(Array *array)
{
    char **new_ptr = realloc(
            array->arr,
            (array->max_size *= ARR_EXTEND_MULT) * sizeof(array->arr[0]));
    if (!new_ptr) {
        return 0;
    }
    array->arr= new_ptr;
    return 1;
}

int
array_add(Array *array, char *new_str)
{
    // Extend if full
    if (array->size >= array->max_size) {
        if (!array_extend(array)) {
            return 0;
        }
    }

    size_t len = strlen(new_str);
    // Allocate memory for the string
    array->arr[array->size] = malloc(len * sizeof(char) + 1);
    if (!array->arr[array->size]) {
        return 0;
    }
    // Now copy the string into the array
    strcpy(array->arr[array->size], new_str);
    ++array->size; 
    return 1;
}
    
void
array_delete(Array *array)
{
    for (size_t i = 0; i < array->size; ++i) {
        free(array->arr[i]);
    }
}
