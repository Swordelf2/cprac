#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Resizable strings, supporting UTF-8
// Note that these are NOT null-terminated
typedef struct String
{
    char *str;
    size_t size;
    size_t max_size;
    size_t cp_size; // length in UTF-8
} String;

enum
{
    STRING_INIT_SIZE = 16,
    STRING_EXTEND_MUL = 2,
    LEFT_BIT_MASK = 0x80u,
    WORD_DELIM = 0x20u,
    CP_MAX_SIZE = 4
};

void
string_init(String *string);

void
string_extend(String *string);

static void
string_append(String *string, char c);

int
string_readword(String *string, FILE *in);
// Returns 0 upon reaching EOF

void
string_refresh(String *string);

void
string_print(String *string, FILE *out);

void
string_free(String *string);

int
main(void)
{
    String max_string;
    size_t max_cp_size = 0;
    String cur_string;
    string_init(&max_string);
    string_init(&cur_string);

    int flag;
    do {
        flag = string_readword(&cur_string, stdin);
        if (cur_string.cp_size > max_cp_size) {
            max_cp_size = cur_string.cp_size;
            // swap max_string and cur_string
            String temp = max_string;
            max_string = cur_string;
            cur_string = temp;
        }
        string_refresh(&cur_string);
    } while (flag);
    
    printf("%zu\n", max_cp_size);
    if (max_cp_size > 0) {
        string_print(&max_string, stdout);
        putchar('\n');
    }
    string_free(&cur_string);
    string_free(&max_string);
    return 0;
}

void
string_init(String *string)
{
    string->str = malloc((string->max_size = STRING_INIT_SIZE));
    string->size = string->cp_size = 0;
}

void
string_extend(String *string)
{
    string->str = realloc(string->str, string->max_size *= STRING_EXTEND_MUL);
}

static void
string_append(String *string, char c)
{
    if (string->size >= string->max_size) {
        string_extend(string);
    }
    string->str[string->size++] = c;
    // Check if the byte is leading
    unsigned char uc = c;
    int not_leading = (uc & LEFT_BIT_MASK) && !(uc & ((unsigned) LEFT_BIT_MASK >> 1));
    if (!not_leading) {
        ++string->cp_size;
    }
}

void
string_refresh(String *string)
{
    string->size = string->cp_size = 0;
}

void
string_free(String *string)
{
    free(string->str);
}

int
string_readword(String *string, FILE *in)
{
    int c;
    while (1) {
        c = getc_unlocked(in);
        if (c == EOF) {
            return 0; // EOF is reached
        }
        if ((unsigned char) c <= WORD_DELIM) {
            return 1; // finished reading a word
        }
        string_append(string, c);
    }
}

void
string_print(String *string, FILE *out)
{
    fwrite(string->str, 1, string->size, out);
}
