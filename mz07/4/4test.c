#include <stdio.h>



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

extern RandomOperations operations;

int
main(void)
{
    RandomGenerator *rr = random_create(1234);
    for (int i = 0; i < 10; ++i) {
        printf("%d ", rr->ops->next(rr));
    }
    rr->ops->destroy(rr);
    return 0;
}
