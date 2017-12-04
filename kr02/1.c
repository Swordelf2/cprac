#include <stdio.h>
#include <linux/limits.h>

// This value suffices because 
// each string contains no more than 255 characters
// plus a path to a file, which is no longer than PATH_MAX
enum
{
    BUFF_MAX_SIZE = PATH_MAX + 256
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
