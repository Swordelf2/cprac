#include <stdio.h>

char *
getline2(FILE *f);

int
main(void)
{
    char *s = getline2(stdin);
    if (s == NULL) {
        printf("s is NULL\n");
    } else {
        fputs(s, stdout);
    }
    return 0;
}
