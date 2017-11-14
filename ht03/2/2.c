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
};

void
MatSymmSqr(double *mat_dest, double *mat_src, int N);
// mat_dest = (mat_src)^2 ; mat_src is required to be symmetrical,
// and to have all zeroes on the main diagonal
// note that only the upper part of mat_src is read

void
MatrixProd(double *mat1, double *mat2, int N);

int 
larger2power(int N);

int
main(int argc, char *argv[])
{
    int in_fd = open(argv[ARG_POS_INFILE], O_RDONLY);
    int out_fd = open(argv[ARG_POS_OUTFILE], O_RDWR | O_CREAT | O_TRUNC, 0644);
    int P = strtol(argv[ARG_POS_P], NULL, DEC_BASE);

    // Map the files
    struct stat file_stat;
    fstat(in_fd, &file_stat);
    size_t file_size = file_stat.st_size;
    // Initialize N
    int N = 0;
    while ((unsigned) N * N < file_size / sizeof(double)) {
        ++N;
    }
    ftruncate(out_fd, N * N * sizeof(double));
    double *in_mem = mmap(NULL, file_size,
            PROT_WRITE | PROT_READ, MAP_PRIVATE, in_fd, 0);
    close(in_fd);
    double *out_mem = mmap(NULL, N * N * sizeof(double),
            PROT_WRITE | PROT_READ, MAP_SHARED, out_fd, 0);
    close(out_fd);
    
    if (N == 0) {
        goto cleanup_unmap;
    }

    if (P == 0) {
        // Print identity matrix
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                if (i == j) {
                    out_mem[i * N + j] = 0.0;
                } else {
                    out_mem[i * N + j] = INFINITY;
                }
            }
        }
        goto cleanup_unmap;
    }

    double *mat_init = in_mem;

    // The matrix is stabilized after N-th power
    if (P >= N) {
        P = larger2power(N);
    }

    double *mat_fast = mmap(NULL, N * N * sizeof(*mat_fast), 
            PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    memcpy(mat_fast, mat_init, N * N * sizeof(*mat_fast));
    double *mat_fast_tmp = mmap(NULL, N * N * sizeof(*mat_fast_tmp), 
            PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
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

    memcpy(out_mem, mat_fast, N * N * sizeof(*out_mem));

cleanup_unmap:
    munmap(in_mem, file_size);
    munmap(out_mem, N * N * sizeof(double));
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

void
MatrixProd(double *mat1, double *mat2, int N)
{
    double *mat_res = mmap(NULL, N * N * sizeof(*mat_res),
            PROT_WRITE | PROT_READ, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
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
    munmap(mat_res, N * N * sizeof(*mat_res));
}

int
larger2power(int N)
{
    unsigned pow = 1;
    while ((int) pow < N) {
        pow <<= 1;
    }
    return pow;
}
