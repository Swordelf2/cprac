#include <stdlib.h>
#include "rand.h"

int rand_range(int low, int high)
{
    double val = rand() / (RAND_MAX + 1.0);
    return low + (int) ((high - low) * val);
}
