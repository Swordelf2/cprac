#include <stdio.h>
#include <stdlib.h>

enum
{
    STRING_INIT_SIZE = 16,
    STRING_EXTEND_MUL = 2
};

// resizable string
typedef struct String
{
    char *str;
    size_t size;
    size_t max_size;
} String;

// All these functions return 0 on failure
int
string_init(String *string);

int
string_extend(String *string);

int
string_append(String *string, char c);

char *
getline2(FILE *f)
{
    int c = getc(f);
    if (c == EOF) {
        return NULL;
    }
    String string;
    if (!string_init(&string)) {
        return NULL;
    }
    while (c != EOF) {
        if (!string_append(&string, c)) {
            return NULL;
        }
        if (c == '\n') {
            break;
        }
        c = getc(f);
    }
    if (!string_append(&string, '\0')) {
        return NULL;
    }
    return string.str;
}

int
string_init(String *string)
{
    if (!(string->str = malloc((string->max_size = STRING_INIT_SIZE)))) {
        return 0;
    }
    string->size = 0;
    return 1;
}

int
string_extend(String *string)
{
    if (!(string->str = realloc(string->str, string->max_size *= STRING_EXTEND_MUL))) {
        return 0;
    }
    return 1;
}

int
string_append(String *string, char c)
{
    if (string->size >= string->max_size) {
        if (!string_extend(string)) {
            return 0;
        }
    }
    string->str[string->size++] = c;
    return 1;
}
