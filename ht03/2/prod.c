#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void
ReadMatrix(double *mat, int N);

void
PrintMatrix(double *mat, int N);

void
MatrixProd(double *mat1, double *mat2, int N);
// Calculate the production of two matrices and store the result in mat1
// the production is defined with "min" and "+"

int
main(void)
{
    int N;
    scanf("%d", &N);
    double *mat1 = malloc(N * N * sizeof(*mat1));
    double *mat2 = malloc(N * N * sizeof(*mat2));
    ReadMatrix(mat1, N);
    ReadMatrix(mat2, N);
    MatrixProd(mat1, mat2, N);
    PrintMatrix(mat1, N);
    return 0;
}
       
    
void
ReadMatrix(double *mat, int N)
{
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            scanf("%lf", &mat[i * N + j]);
        }
    }
}


void
PrintMatrix(double *mat, int N)
{
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            printf("%5.f ", mat[i * N + j]);
        }
        putchar('\n');
    }
}

void
MatrixProd(double *mat1, double *mat2, int N)
{
    double *mat_res = malloc(N * N * sizeof(*mat_res));
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            double min = +INFINITY;
            for (int s = 0; s < N; ++s) {
                double sum = mat1[i * N + s] + mat2[s * N + j];
                if (sum < min) {
                    min = sum;
                }
            }
            mat_res[i * N + j] = min;
        }
    }
    memcpy(mat1, mat_res, N * N * sizeof(*mat1));
}
