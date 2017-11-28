#include <stdio.h>
#include <time.h>
#include <string.h>

int
main(void)
{
    int year;
    int month;
    int wday; 
    scanf("%d %d %d", &year, &month, &wday);
    struct tm t;

    memset(&t, 0, sizeof(t));
    t.tm_mon = month - 1;
    t.tm_year = year - 1900;
    t.tm_mday = 1;

    time_t tim = mktime(&t);
    struct tm *lt = localtime(&tim);
    int first_day = lt->tm_wday;

    memset(&t, 0, sizeof(t));
    t.tm_mon = month;
    t.tm_year = year - 1900;
    t.tm_mday = 0;

    tim = mktime(&t);
    lt = localtime(&tim);
    int day_count = lt->tm_mday;

    int tonext = wday - first_day;
    if (tonext < 0) {
        tonext += 7;
    }
    int x = 1 + tonext;
    while (x <= day_count) {
        printf("%d\n", x);
        x += 7;
    }

    return 0;
}
