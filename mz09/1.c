#include <stdio.h>
#include <stdint.h>

enum
{
    BUFF_SIZE = 8192,
    SHIFT_LENGTH = 22,
    TABLE_SIZE = 1024,
    PAGE_SIZE = 4096
};

int
main(void)
{
    char str[BUFF_SIZE];
    uint32_t left, right, last;
    last = 0;
    unsigned t_count = 0; // tables count;
    while (fgets(str, sizeof(str), stdin)) {
        sscanf(str, "%x-%x", &left, &right);
        --right;
        left >>= SHIFT_LENGTH;
        right >>= SHIFT_LENGTH;
        t_count += right - left + 1;
        if (left == last && last != 0) {
            --t_count;
        }
        last = right;
    }

    printf("%u\n", (t_count + 1) * PAGE_SIZE);
    return 0;
}
