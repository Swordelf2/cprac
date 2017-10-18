#include <stdio.h>
#include <wchar.h>
#include <wctype.h>

int
main(void)
{
    wint_t c;
    unsigned long long digs = 0;
    unsigned long long ups = 0;
    unsigned long long lows = 0;

    while ((c = getwc(stdin)) != WEOF) {
        if (iswalpha(c)) {
            ++digs;
        } else if (iswupper(c)) {
            ++ups;
        } else if (iswlower(c)) {
            ++lows;
        }
    }
    printf("%llu %llu %llu\n", digs, ups, lows);
    printf("%zu", sizeof(wchar_t));
    return 0;
}
