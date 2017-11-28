#include <stdio.h>

enum
{
    BUFF_MAX_SIZE = 8192
};

int
main(void)
{
    char str[BUFF_MAX_SIZE];
    unsigned wsum = 0;
    unsigned xsum = 0;
    while (fgets(str, sizeof(str), stdin)) {
        unsigned left, right;
        char r, w, x;
        sscanf(str,"%x-%x %c%c%c", &left, &right, &r, &w, &x);
        if (w == 'w') {
            wsum += right - left;
        }
        if (x == 'x') {
            xsum += right - left;
        }
    }

    printf("%u\n%u\n", wsum, xsum);
    return 0;
}
