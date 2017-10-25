#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <locale.h>
// resizable wide char string
// note that hese are NOT null-terminated
typedef struct WString
{
    wchar_t *str;
    size_t size;
    size_t max_size;
} WString;

enum
{
    WSTR_INIT_SIZE = 16,
    WSTR_EXTEND_MUL = 2,
    WORD_DELIM = 0x20u
};

void
wstring_init(WString *wstring);

void
wstring_extend(WString *wstring);

void
wstring_append(WString *wstring, wchar_t wc);

void
wstring_destroy(WString *wstring);

int
compare(const void *arg1, const void *arg2, void *len);

int
main(void)
{
    if(!setlocale(LC_CTYPE, "en_US.UTF-8")) {
        fputs("Could not set locale", stderr);
    }
    WString wstring;
    wstring_init(&wstring);
    wint_t wc;
    while ((wc = getwchar()) != WEOF) {
        if ((unsigned) wc > WORD_DELIM) {
            wstring_append(&wstring, wc);
        }
    }

    // extend enough to place a copy right after it
    while (wstring.max_size < wstring.size * 2) {
        wstring_extend(&wstring);
    }
    // now place that copy
    wcsncpy(wstring.str + wstring.size, wstring.str, wstring.size);
    
    // array of pointers to the string
    wchar_t **parr;
    parr = malloc(wstring.size * sizeof(parr[0]));
    for (size_t i = 0; i < wstring.size; ++i) {
        parr[i] = wstring.str + i;
    }

    // set the global variable for the qsort to work
    qsort_r(parr, wstring.size, sizeof(parr[0]), compare, &wstring.size);

    for (size_t i = 0; i < wstring.size; ++i) {
        putwchar(parr[i][wstring.size - 1]);
    }
    putwchar(L'\n');

    free(parr);
    wstring_destroy(&wstring);

    return 0;
}

void
wstring_init(WString *wstring)
{
    wstring->str = malloc(WSTR_INIT_SIZE * sizeof(wstring->str[0]));
    wstring->size = 0;
    wstring->max_size = WSTR_INIT_SIZE;
}

void
wstring_extend(WString *wstring)
{
    wstring->str = realloc(wstring->str,
            (wstring->max_size *= WSTR_EXTEND_MUL) * sizeof(wstring->str[0]));
}

void
wstring_append(WString *wstring, wchar_t wc)
{
    if (wstring->size >= wstring->max_size) {
        wstring_extend(wstring);
    }
    wstring->str[wstring->size++] = wc;
}

void
wstring_destroy(WString *wstring)
{
    free(wstring->str);
}

int compare(const void *arg1, const void *arg2, void *len)
{
    return wcsncmp(*(const wchar_t **) arg1, *(const wchar_t **) arg2, *(size_t *) len);
}
