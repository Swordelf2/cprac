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

int
main(int argc, char *argv[])
{
    if (argc < 2) {
        printf("Specify at least 1 argmuent!\n");
        exit(1);
    }
    String string1;
    string_init(&string1);
    string_assign(&string1, argv[1]);
    string_print(&string1);
    putchar('\n');
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
    memcpy(string->str, str, len);
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
