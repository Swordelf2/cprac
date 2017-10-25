#include <stdlib.h>
#include <stdio.h>

enum
{
    COUNT_POS = 1,
    LOW_POS = 2,
    HIGH_POS = 3,
    SEED_POS = 4
};

int
main(int argc, char *argv[])
{
    if (argc < 5) {
        return -1;
    }
    srand(atoi(argv[SEED_POS]));
    int low = atoi(argv[LOW_POS]);
    int high = atoi(argv[HIGH_POS]);
    int count = atoi(argv[COUNT_POS]);
    for (int i = 0; i < count; ++i) {
        double val = rand() / (RAND_MAX + 1.0);
        int rand_res = low + (int) ((high - low) * val);
        printf("%d\n", rand_res);
    }

    return 0;
}
