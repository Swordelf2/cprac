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
    RandomOperations *ops;

};

RandomGenerator *
random_create(int seed);

unsigned
random_generator_next(RandomGenerator *rr);

void
random_generator_destroy(RandomGenerator *rr);

const RandomOperations operations = {random_generator_next, random_generator_destroy};

RandomGenerator *random_create(int seed)
{
    RandomGenerator *new = calloc(1, sizeof(*new));
    new->curX = seed;
    new->ops = (void *) &operations;
    return new;
}

unsigned
random_generator_next(RandomGenerator *rr)
{
    return rr->curX = (RR_MUL * rr->curX + RR_INCR) % RR_MOD;
}

void
random_generator_destroy(RandomGenerator *rr)
{
    free(rr);
}
