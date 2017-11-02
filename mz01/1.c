#include <stdio.h>
#include <string.h>
#include <ctype.h>

enum
{
    BUFFER_SIZE = 62,
    DIGIT_COUNT = 10
};

int 
main(void)
{
    char filename[BUFFER_SIZE];
    int file_empty = 0;

    int c;
    int len = 0;
    while (1) {
        c = getchar_unlocked();
        len++;
        if (len > BUFFER_SIZE) {
            --len;
            file_empty = 1;
            break;
        }
        if (c == '\n' || c == '\r' || c == EOF) {
            break;
        }
        filename[len - 1] = c;
    }

    unsigned num_count[DIGIT_COUNT];
    memset(num_count, 0, sizeof(num_count));

    if (!file_empty) {
        filename[len - 1] = '\0';
        FILE *in_file = fopen(filename, "rb");
        if (in_file) {
            while ((c = fgetc_unlocked(in_file)) != EOF) {
                if (isdigit(c)) {
                    num_count[c - '0']++;
                }
            }
            fclose(in_file);
        }
    }

    for (int i = 0; i < DIGIT_COUNT; ++i) {
        printf("%d %u\n", i, num_count[i]); 
    }

    return 0;
}
