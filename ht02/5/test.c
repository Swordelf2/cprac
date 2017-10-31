#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>

#include <stdio.h>
#include <stdlib.h>

ssize_t getcwd2(int fd, char *buf, size_t size);

enum
{
    BUFF_SIZE = 2
};

int
main(int argc, char *argv[])
{
    char *buff = malloc(BUFF_SIZE);
    DIR *my_dir = opendir(argv[1]);
    ssize_t result = getcwd2(dirfd(my_dir), buff, BUFF_SIZE);
    closedir(my_dir);
    puts(buff);
    printf("return value = %d\n", result);
    free(buff);
    return 0;
}
