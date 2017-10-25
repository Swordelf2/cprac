typedef struct RandomGenerator
{
    unsigned curX;
    RandomOperations *ops;
} RandomGenerator;

struct RandomOperations
{
    unsigned (*next)(RandomGenerator *rr);
    void (*destroy)(RandomGenerator *rr);
}

