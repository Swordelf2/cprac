#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

enum
{
    BITS_IN_BYTE = 8,
    BIT_SIZE = 63
};

int64_t
calc_max_size(int64_t block_size, 
        int64_t block_num_size,
        int64_t inode_direct_block_count);

static inline int64_t
get_max_size(int64_t bs, int64_t bns, int64_t idbc);
// returns INT64_MAX in case of overflow

int
main(void)
{
    int64_t bs, bns, idbc;
    scanf("%"SCNd64" %"SCNd64" %"SCNd64, &bs, &bns, &idbc);
    
    printf("%"PRId64"\n", get_max_size(bs, bns, idbc));

    // Find the best bns
    int flag = 1;
    int64_t prev;
    int64_t cur;
    for (bns = 1; flag; ++bns) {
        int64_t overall;
        int pow2 = BITS_IN_BYTE * bns;
        if (pow2 < BIT_SIZE) {
            overall = bs * ((uint64_t) 0x1) << pow2;
        } else {
            overall = INT64_MAX;
        }

        cur = get_max_size(bs, bns, idbc);
        if (cur <= overall) {
            flag = 0;
            break;
        }
        prev = overall;
    }
    if (cur > prev || bns == 1) {
        printf("%"PRId64" %"PRId64"\n", bns, cur);
    } else {
        printf("%"PRId64" %"PRId64"\n", bns - 1, prev);
    }
    return 0;
}

int64_t calc_max_size(int64_t block_size, 
        int64_t block_num_size,
        int64_t inode_direct_block_count)
{
    // The number of block pointers that can be stored in one block
    int64_t pcount = block_size / block_num_size;
    int64_t sqr, cube;
    // The formula
    /*
    result = block_size *
            (inode_direct_block_count +
            pcount +
            pcount * pcount +
            pcount * pcount * pcount);
    */
    int64_t tmp;
    if (__builtin_mul_overflow(pcount, pcount, &sqr)) {
        return -1;
    }
    if (__builtin_mul_overflow(sqr, pcount, &cube)) {
        return -1;
    }
    if (__builtin_add_overflow(cube, sqr, &tmp)) {
        return -1;
    }
    if (__builtin_add_overflow(tmp, pcount, &tmp)) {
        return -1;
    }
    if (__builtin_add_overflow(tmp, inode_direct_block_count, &tmp)) {
        return -1;
    }
    if (__builtin_mul_overflow(tmp, block_size, &tmp)) {
        return -1;
    }
    return tmp;
}

static inline int64_t
get_max_size(int64_t bs, int64_t bns, int64_t idbc)
{
    int64_t res = calc_max_size(bs, bns, idbc);
    if (res == -1) {
        return INT64_MAX;
    }
    return res;
}
