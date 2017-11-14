#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>

enum
{
    ARG_POS_INFILE = 1,
    ARG_POS_OUTFILE = 2,
    ARG_POS_P = 3,
    DEC_BASE = 10,
    POWS_MAX_COUNT = sizeof(unsigned)
};

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
            printf("%6.f ", mat[i * N + j]);
        }
        putchar('\n');
    }
}

void
MatSymmSqr(double *mat_dest, double *mat_src, int N);
// mat_dest = (mat_src)^2 ; mat_src is required to be symmetrical,
// and to have all zeroes on the main diagonal
// note that only the upper part of mat_src is read

int
main(void)
{
    /*
    int in_fd = open(argv[ARG_POS_INFILE], O_RDONLY);
    int out_fd = creat(argv[ARG_POS_OUTFILE], 0644);
    int P = strtol(argv[ARG_POS_P], NULL, DEC_BASE);

    // map the files
    off_t in_file_size, out_file_size;
    struct stat file_stat;
    fstat(in_fd, &file_stat);
    in_file_size = file_stat.st_size;
    fstat(out_fd, &file_stat);
    out_file_size = file_stat.st_size;
    double *in_mem = mmap(NULL, in_file_size, PROT_READ, MAP_SHARED, in_fd, 0);
    close(in_fd);
    double *out_mem = mmap(NULL, out_file_size, PROT_WRITE, MAP_SHARED, out_fd, 0);
    close(out_fd);
    
    munmap(in_mem, in_file_size);
    munmap(out_mem, out_file_size);
    */
    unsigned P;
    printf("Enter P: ");
    scanf("%u", &P);
    unsigned N;
    printf("Enter N: ");
    scanf("%u", &N);
    if (N == 0 || P == 0) {
        // TODO: print the identity matrix
        return 0;
    }
    // The matrix is stabilized after N-th power
    double *mat_init = malloc(N * N * sizeof(*mat_init));
    printf("Enter the matrix:\n");
    ReadMatrix(mat_init, N);

    // Correct one, but slow
    double *mat_good = malloc(N * N * sizeof(*mat_good));
    memcpy(mat_good, mat_init, N * N * sizeof(*mat_good));
    for (unsigned i = 0; i < P - 1; ++i) {
        MatrixProd(mat_good, mat_init, N);
    }

    if (P >= N) {
        P = N;
    }

    // Fast one
    // pows is an array of matrices pows[i] == mat ^ (2 ^ i)
    double *mat_fast = malloc(N * N * sizeof(*mat_fast));
    memcpy(mat_fast, mat_init, N * N * sizeof(*mat_fast));
    double *mat_fast_tmp = malloc(N * N * sizeof(*mat_fast));
    --P;
    while (P) {
        if (P % 2) {
            MatrixProd(mat_fast, mat_init, N);
            --P;
        } else {
            MatSymmSqr(mat_fast_tmp, mat_fast, N);
            // Swap mat_fast_tmp and mat_fast (pointers)
            double *tmp = mat_fast;
            mat_fast = mat_fast_tmp;
            mat_fast_tmp = tmp;
            P /= 2;
        }
    }

    printf("Correct matrix:\n");
    PrintMatrix(mat_good, N);
    free(mat_good);
    printf("\nFast matrix:\n");
    PrintMatrix(mat_fast, N);
    free(mat_fast);

    if (memcmp(mat_good, mat_fast, N * N * sizeof(*mat_good)) == 0) {
        printf("\nThe resulting matrices are EQUAL\n");
    } else {
        printf("\nThe resulting matrices are NOT EQUAL\n");
    }
    return 0;
}

void
MatSymmSqr(double *mat_dest, double *mat_src, int N)
{
    for (int i = 0; i < N - 1; ++i) {
        for (int j = i + 1; j < N; ++j) {
            double min = +INFINITY;
            for (int s = 0; s < N; ++s) {
                double sum = mat_src[i * N + s] + mat_src[j * N + s];
                if (sum < min) {
                    min = sum;
                }
            }
            mat_dest[i * N + j] = min;
        }
    }
    // Zeroes on the main diagonal
    for (int i = 0; i < N; ++i) {
        mat_dest[i * N + i] = 0.0;
    }
    // Mirror the lower half of the matrix
    for (int i = 1; i < N; ++i) {
        for (int j = 0; j < i; ++j) {
            mat_dest[i * N + j] = mat_dest[j * N + i];
        }
    }
}
