#include <stdio.h>

void normalize_path(char *buf);

int 
main(void)
{
    char buf[256];
    fgets(buf, 256, stdin);
    buf[strlen(buf) - 1] = '\0';
    normalize_path(buf);
    puts(buf);
    return 0;
}
