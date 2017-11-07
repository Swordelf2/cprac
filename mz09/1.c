#include <stdio.h>
#include <stdint.h>

enum
{
    MAX_STR_SIZE = 8192,
    PAGE_SIZE = 4096
};

int
main(void)
{
    char str[MAX_STR_SIZE];
    uint32_t sum_size = 0;
    
    unsigned page_count = 0;

    while (fgets(str, sizeof(str), stdin)) {
        uint32_t left, right;
        sscanf(str, "%x-%x", &left, &right);
        sum_size += right - left;
        page_count += (right - left - 1) / PAGE_SIZE + 1;
    }

    printf("%u\n", page_count);
    return 0;
}
