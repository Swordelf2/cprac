#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int main(int argc, char *argv[])
{
    for (int i = 1; i < argc; ++i) {
        char *last = argv[i];
        char *newlast;
        long long umin = INT_MAX;
        long long umax = INT_MIN;
        while (*last != '\0') {
            if (*last == ',') {
                ++last;
            }
            int a = strtol(last, &newlast, 10);
            if (a < umin) {
                umin = a;
            }
            if (a > umax) {
                umax = a;
            }
            last = newlast;
        }
        printf("%lld\n",  umax - umin + 1);
    }
    return 0;
}
