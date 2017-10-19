#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void normalize_path(char *buf);

int 
main(void)
{
    char *buf = malloc(256);
    fgets(buf, 256, stdin);
    buf[strlen(buf) - 1] = '\0';
    normalize_path(buf);
    puts(buf);
    free(buf);
    return 0;
}
