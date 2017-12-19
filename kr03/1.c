#include <stdio.h>
#include <math.h>

struct RoundResult
{
    long long value;
    int status;
};

long long
rem(long long val, long long div)
{
    long long res = val % div;
    if (val < 0) {
        res = div + res;
        if (res == div) {
            res = 0;
        }
    }
    return res;
}

struct RoundResult
iround(long long value, int prec)
{
    long long div = 1;
    struct RoundResult res = {0};
    for (int i = 0; i < prec; ++i) {
        div *= 10;
    }
    long long mod = rem(value, div);
    if ((mod < div / 2) || div == 1) {
        // round down
        if (__builtin_add_overflow(value, -mod, &res.value)) {
            res.status = -1;
            res.value = 0;
        }
    } else {
        // round up
        if (__builtin_add_overflow(value, div - mod, &res.value)) {
            res.status = 1;
            res.value = 0;
        }
    }
    return res;
}
