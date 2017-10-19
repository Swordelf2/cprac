#include <stdio.h>

enum
{
    BIT_WIDTH = 7,
    S_BIT = 1 << BIT_WIDTH,
    MASK = S_BIT - 1
};

int
main(void)
{
    unsigned a;
    while (scanf("%u", &a) == 1) {
        if (a == 0) {
            printf("00\n");
        } else {
            while (a >> BIT_WIDTH) {
                printf("%02x ", (a & MASK) | S_BIT);
                a >>= BIT_WIDTH;
            }
            printf("%02x\n", a & MASK);
        }
    }
    return 0;
}
