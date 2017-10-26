#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <stdio.h>

int
main(void)
{
    struct stat my_stat;
    ino_t my_ino;
    stat("hey1", &my_stat);
    my_ino = my_stat.st_ino;
    stat("hey2", &my_stat);
    printf("%d %d\n", my_ino, my_stat.st_ino);
    return 0;
}
