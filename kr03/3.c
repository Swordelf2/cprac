#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/mman.h>

#include <arpa/inet.h>

typedef struct Pair
{
    int key;
    int value;
} Pair;

int
main(int argc, char *argv[])
{
    int key = strtol(argv[2], NULL, 0);
    
    struct Pair pair = (key, 0);

    int fd = open(argv[1], O_RDONLY);
    size_t size;
    size = lseek(fd, 0, SEEK_END);
    
    Pair *base = mmap(NULL, size, PROT_READ, MAP_PRIVATE, fd, 0);
    
    Pair *ptr =
        bsearch(&pair, base, size / sizeof(Pair), sizeof(Pair), compare);
    if (ptr == NULL) {
        printf("0 0\n");
    } else {
        int val = ntohl(ptr->value);
        printf("1 %d\n", val);
    }
    return 0;
}

int
compare(const void *arg1, const void *arg2)
{
    Pair *p1 = (void *)arg1;
    Pair *p2 = (void *)arg2;
    int key1 = ntohl(p1->key);
    int key2 = ntohl(p2->key);
    if (key1 > key2) {
        return 1;
    } else if (key1 == key2) {
        return 0;
    } else {
        return -1;
    }
}
