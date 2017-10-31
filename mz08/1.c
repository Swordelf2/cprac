#include <stdio.h>
#include <stdint.h>

enum
{
    MAX_STR_SIZE = 8192
};

int
main(void)
{
    char str[MAX_STR_SIZE];
    uint32_t sum_size = 0;
    
    while (fgets(str, sizeof(str), stdin)) {
        uint32_t left, right;
        sscanf(str, "%x-%x", &left, &right);
        sum_size += right - left;
    }

    printf("%u\n", sum_size);
    return 0;
}
