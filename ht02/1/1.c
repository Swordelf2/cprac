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

void
string_init(String *string);

void
string_extend(String *string);

void
string_append(String *string, char c);

char *
getline2(FILE *f)
{
    int c = getc(f);
    if (c == EOF) {
        return NULL;
    }
    String string;
    string_init(&string);
    while (c != EOF) {
        string_append(&string, c);
        if (c == '\n') {
            break;
        }
        c = getc(f);
    }
    string_append(&string, '\0');
    return string.str;
}

void
string_init(String *string)
{
    string->str = malloc((string->max_size = STRING_INIT_SIZE));
    string->size = 0;
}

void
string_extend(String *string)
{
    string->str = realloc(string->str, string->max_size *= STRING_EXTEND_MUL);
}

void
string_append(String *string, char c)
{
    if (string->size >= string->max_size) {
        string_extend(string);
    }
    string->str[string->size++] = c;
}
