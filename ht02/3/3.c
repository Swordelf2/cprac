#include <stdio.h>
#include <wchar.h>
#include <wctype.h>
#include <errno.h>
#include <string.h>
#include <locale.h>

int
main(void)
{
    wint_t c;
    unsigned long long digs = 0;
    unsigned long long ups = 0;
    unsigned long long lows = 0;

    if(!setlocale(LC_CTYPE, "en_US.UTF-8")) {
        fputs("Could not set locale\n", stdout);
    }

    while ((c = getwc(stdin)) != WEOF) {
        if (iswdigit(c)) {
            ++digs;
        } else if (iswupper(c)) {
            ++ups;
        } else if (iswlower(c)) {
            ++lows;
        }
    }
    printf("%llu\n%llu\n%llu\n", digs, ups, lows);
    return 0;
}
