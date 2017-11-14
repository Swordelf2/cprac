// filemaker
#include <stdio.h>
#include <stdlib.h>

int
main(void)
{
    // makes the symmetrical matrix N * N of doubles
    FILE *f = fopen("in", "w+");
    int N;
    printf("Enter N: ");
    scanf("%d", &N);
    double *mat = malloc(N * N * sizeof(*mat));
    printf("Enter doubles:\n");
    for (int i = 0; i < N - 1; ++i) {
        for (int j = i + 1; j < N; ++j) {
            scanf("%lf", &mat[i * N + j]);
            mat[j * N + i] = mat[i * N + j];
        }
    }
    for (int i = 0; i < N; ++i) {
        mat[i * N + i] = 0.0f;
    }

    fwrite(mat, sizeof(*mat), N * N, f);
    fclose(f);
    return 0;
}
