#include <stdio.h>

int
mysys(const char *str);

int
main(int argc, char *argv[])
{
    printf("%d\n", mysys(argv[1]));
    return 0;
}   
