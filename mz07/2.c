#include <stdio.h>
#include <stdlib.h>

typedef struct RandVal
{
    unsigned beg;
    unsigned len;
    int val;
} RandVal;

enum
{
    ARR_MAX_SIZE = 100
};

int
main(void)
{
    RandVal arr[ARR_MAX_SIZE];
    unsigned n;
    scanf("%u", &n);
    unsigned cur_pos = 0;
    for (unsigned i = 0; i < n; ++i) {
        int val;
        unsigned prob;
        scanf("%d %u", &val, &prob);
        arr[i].beg = cur_pos;
        arr[i].len = prob;
        arr[i].val = val;
        cur_pos += prob;
    }
    int m;
    scanf("%d", &m);
    unsigned seed;
    scanf("%u", &seed);
    srand(seed);
    for (int i = 0; i < m; ++i) {
        double rval = (rand() / (RAND_MAX + 1.0)) * 100.0;
        for (unsigned j = 0; j < n; ++j) {
            if (rval >= arr[j].beg && rval < arr[j].beg + arr[j].len) {
                printf("%d\n", arr[j].val);
                break;
            }
        }
    }

    return 0;
}

