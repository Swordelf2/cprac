#include <stdlib.h>

enum
{
    RR_MUL = 1103515245u,
    RR_INCR = 12345u,
    RR_MOD = 0x80000000u
};

typedef struct RandomOperations RandomOperations;
typedef struct RandomGenerator RandomGenerator;

struct RandomOperations
{
    unsigned (*next)(RandomGenerator *rr);
    void (*destroy)(RandomGenerator *rr);
}; 

struct RandomGenerator
{
    unsigned curX;
    unsigned mul;
    unsigned incr;
    unsigned mod;
    RandomOperations *ops;

};

RandomGenerator *
random_create(int seed);

unsigned
random_generator_next(RandomGenerator *rr);

void
random_generator_destroy(RandomGenerator *rr);

RandomOperations operations = {random_generator_next, random_generator_destroy};

RandomGenerator *random_create(int seed)
{
    RandomGenerator *new = malloc(sizeof(RandomGenerator));
    new->curX = seed;
    new->mul = RR_MUL;
    new->incr = RR_INCR;
    new->mod = RR_MOD;
    new->ops = &operations;
    return new;
}

unsigned
random_generator_next(RandomGenerator *rr)
{
    return rr->curX = (rr->mul * rr->curX + rr->incr) % rr->mod;
}

void
random_generator_destroy(RandomGenerator *rr)
{
    free(rr);
}
