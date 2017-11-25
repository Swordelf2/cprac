#include <stdio.h>
#include <math.h>

int
main(int argc, char *argv[])
{
    FILE *file = fopen(argv[1], "r");
    fseek(file, 0l, SEEK_END);
    size_t file_size = ftell(file);
    rewind(file);
    int N = sqrt(file_size / sizeof(double));
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            double x;
            fread(&x, sizeof(x), 1, file);
            printf("%8.2f ", x);
        }
        putchar('\n');
    }
    return 0;
}
