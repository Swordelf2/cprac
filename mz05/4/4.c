#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>


enum
{
    ARR_EXTEND_MULT = 2, // resizable array extension multiplier
    ARR_INIT_SIZE = 16 // TODO: change this value!
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
array_sort(Array *array);

void
array_delete(Array *array);

int
walk_with_cd(char *root_path, char *dir_name);

int
compare_func(const void *arg1, const void *arg2);

int
main(int argc, char *argv[])
{
    if (argc > 1) {
        walk_with_cd(argv[1], NULL);
    } else {
        printf("Specify a path\n");
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
array_sort(Array *array)
{
    qsort(array->arr, array->size, sizeof(array->arr[0]), compare_func);
}
    
void
array_delete(Array *array)
{
    for (size_t i = 0; i < array->size; ++i) {
        free(array->arr[i]);
    }
    free(array->arr);
}

int
walk_with_cd(char *root_path, char *dir_name)
{
    DIR *cur_dir = opendir(root_path);
    if (!cur_dir) {
        return 1; // not an error
    }
    // Print "cd" into this dirctory
    if (dir_name != NULL) {
        printf("cd %s\n", dir_name);
    }

    // Initiate the list of names of all files in this directory
    Array array;
    if (!array_init(&array)) {
        return 0;
    }

    // Copy the root_path into a buffer, which will later be appended with filenames
    char full_path[PATH_MAX];
    strcpy(full_path, root_path);
    size_t root_path_len = strlen(full_path);
    full_path[root_path_len++] = '/';

    while (1) {
        // Read the next entry in the directory
        errno = 0;
        struct dirent *dir_entry = readdir(cur_dir);
        if (!dir_entry) {
            if (errno == 0) {
                break;
            } else {
                return 0;
            }
        }
        // Check for a couple of conditions on the name of a file
        if (strcmp(dir_entry->d_name, ".") == 0 ||
                strcmp(dir_entry->d_name, "..") == 0 ||
                strlen(dir_entry->d_name) + root_path_len > PATH_MAX - 1) {
            continue;
        }
        // Finally add it to the array
        if (!array_add(&array, dir_entry->d_name)) {
            array_delete(&array);
            return 0;
        }
    }
    closedir(cur_dir);

    array_sort(&array);
    
    for (size_t i = 0; i < array.size; ++i) {
        strcpy(full_path + root_path_len, array.arr[i]);
        struct stat cur_stat;
        int stat_ret = lstat(full_path, &cur_stat);
        if (stat_ret == -1) {
            continue;
        }
        
        if (S_ISDIR(cur_stat.st_mode)) {
            // Recursively process folded directories
            walk_with_cd(full_path, array.arr[i]);
        }
    }
    array_delete(&array);
    // print "cd" out of this directory
    if (dir_name != NULL) {
        printf("cd ..\n");
    }
    return 1;
}

int
compare_func(const void *arg1, const void *arg2)
{
    return strcasecmp(*(const char **) arg1, *(const char **) arg2);
}
