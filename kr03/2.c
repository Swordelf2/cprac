#include <stdio.h>

int
main(void)
{
    unsigned char a[8];
    unsigned x;
    int flag = 1;
    while (flag) {
        for (int i = 0; i < 8; ++i) {
            if (scanf("%x", &x) != 1) {
                flag = 0;
                break;
            }
            a[i] = x;
        }
        if (flag) {
            unsigned char b;
            for (int i = 0; i < 8; ++i) {
                b = 0;
                for (int j = 0; j < 8; ++j) {
                    // out[i][7-j] = a[j][i]
                    unsigned bit = (a[j] >> i) & 0x1u;
                    b |= bit << (7 - j);
                }
                printf("%x ", (unsigned) b);
            }
            putchar('\n');
        }
    }
    return 0;
}
