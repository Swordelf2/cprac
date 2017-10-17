#include <stdio.h>

enum
{
    L_MASK = 0x1u,
    S_MASK = 0x80u
};

int
main(void)
{
    unsigned a;
    int first;
    while (scanf("%u", &a) == 1) {
        unsigned stck[5] = {0};
        for (int i = 0; i < 5; ++i) {
            for (int j = 0; j < 7; ++j) {
                if ((i * 7) + j >= 32) {
                    goto for_break;
                }
                stck[i] |= a & (L_MASK << j);
            }
            a >>= 7;
        }
for_break:
        
        for (first = 4; (first > 0) && stck[first] == 0 ; --first);

        for (int i = 0; i <= first - 1; ++i) {
            printf(" %02x", stck[i] | S_MASK);
        }
        printf(" %02x\n", stck[first]);
    }
    return 0;
}
