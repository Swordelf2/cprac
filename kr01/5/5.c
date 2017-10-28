#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// resizable string container

typedef struct String
{
    char *str;
    size_t size;
    size_t max_size;
} String;

enum
{
    STR_INIT_SIZE = 16,
    STR_EXTEND_MUL = 2,
    EXPECTED_ARG_COUNT = 4,
    DNAME_ARG_POS = 1,
    SIZE_ARG_POS = 2,
    SUFF_ARG_POS = 3
};

// all string functions return 1 on success and 0 on failure
int
string_init(String *string);

int
string_extend(String *string);

int
string_assign(String *string, char *str);
// duplicates str into string

int
string_compare(String *string1, String *string2);

int
string_has_suffix(String *string, String *suffix);

void
string_print(String *string);

void
string_free(String *string);

static size_t
min(size_t a, size_t b);

static void
print_error(void);

int
main(int argc, char *argv[])
{
    if (argc < EXPECTED_ARG_COUNT) {
        goto exit;
    }
    DIR *dir = opendir(argv[DNAME_ARG_POS]);
    if (!dir) {
        goto exit;
    }

    String suffix, min_name, cur_name;
    if (!string_init(&suffix) || !string_init(&min_name) || !string_init(&cur_name)) {
        goto cleanup_str;
    }
    // Store the suffix into a String container
    if (!string_assign(&suffix, argv[SUFF_ARG_POS])) {
        goto cleanup_str;
    }


    off_t max_file_size = 0;
    int max_init_flag = 0;
    struct dirent *dir_ent;
    unsigned long long sum_size = 0;
    while (1) {
        errno = 0;
        dir_ent = readdir(dir);
        // Error handling
        if (!dir_ent) {
            if (errno != 0) {
                print_error();
                goto cleanup_str;
            } else {
                // All dir_ents are read
                break;
            }
        }
        
        char file_full_path[PATH_MAX];
        // Construct full path to the file
        int prf_ret = snprintf(file_full_path,
                sizeof(file_full_path),
                "%s/%s",
                argv[DNAME_ARG_POS],
                dir_ent->d_name);
        if (prf_ret < 0) {
            goto cleanup_str;
        } else if ((unsigned) prf_ret > sizeof(file_full_path) - 1) {
            // Skip this file because the resulting file path is too long
            continue;
        }
        
        struct stat file_stat;
        if (lstat(file_full_path, &file_stat) != 0) {
            print_error();
            goto cleanup_str;
        }
        if (S_ISREG(file_stat.st_mode)) {
            sum_size += file_stat.st_size;
            if (!string_assign(&cur_name, dir_ent->d_name)) {
                goto cleanup_str;
            }
            if (string_has_suffix(&cur_name, &suffix)) {
                if (file_stat.st_size > max_file_size ||
                        (file_stat.st_size == max_file_size &&
                        string_compare(&cur_name, &min_name) < 0) ||
                        !max_init_flag) {
                    max_init_flag = 1;
                    // Swap min_name and cur_name
                    String temp = min_name;
                    min_name = cur_name;
                    cur_name = temp;
                    max_file_size = file_stat.st_size;
                }
            }
        }
    }
    
    // TODO: max size allowed may be a negative value
    if (sum_size > strtoull(argv[SIZE_ARG_POS], NULL, 10) && max_init_flag) {
        string_print(&min_name);
    }

cleanup_str:
    string_free(&suffix);
    string_free(&min_name);
    string_free(&cur_name);
    closedir(dir);
exit:
    return 0;

}

int
string_init(String *string)
{
    string->max_size = STR_INIT_SIZE;
    string->str = malloc(string->max_size);
    if (!string->str) {
        return 0;
    }
    string->size = 0;
    return 1;
}

int
string_extend(String *string)
{
    string->max_size *= STR_EXTEND_MUL;
    char *new_ptr = realloc(string->str, string->max_size);
    if (!new_ptr) {
        return 0;
    }
    string->str = new_ptr;
    return 1;
}

int
string_assign(String *string, char *str)
{
    size_t len = strlen(str);
    while (string->max_size < len) {
        if (!string_extend(string)) {
            return 0;
        }
    }
    strcpy(string->str, str); // TODO: MISTAKE HERE ('\0' in the end??)
    string->size = len;
    return 1;
}

int
string_compare(String *string1, String *string2)
{
    int cmp_ret = strncmp(string1->str, string2->str, min(string1->size, string2->size));
    if (cmp_ret == 0) {
        if (string1->size > string2->size) {
            return 1;
        }
        if (string1->size < string2->size) {
            return -1;
        }
        return 0;
    }
    return cmp_ret;
}

int
string_has_suffix(String *string, String *suffix)
{
    if (string->size < suffix->size) {
        return 0;
    }
    // compare last suffix->size bytes of string with suffix
    if (strncmp(string->str + string->size - suffix->size, suffix->str, suffix->size) == 0) {
        return 1;
    } else {
        return 0;
    }
}

void
string_print(String *string)
{
    fwrite(string->str, 1, string->size, stdout);
}

void
string_free(String *string)
{
    free(string->str);
}

static size_t
min(size_t a, size_t b)
{
    return a < b ? a : b;
}

static void
print_error(void)
{
    fputs(strerror(errno), stderr);
}
