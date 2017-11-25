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

int
larger2power(int x);

void
MatrixMul(double *mat_dest, double *mat_src, double *mat_tmp, int N);



int
main(int argc, char *argv[])
{
    int in_fd = open(argv[ARG_POS_INFILE], O_RDONLY);
    int out_fd = open(argv[ARG_POS_OUTFILE], O_RDWR | O_CREAT | O_TRUNC, 0777);
    int P = strtol(argv[ARG_POS_P], NULL, DEC_BASE);
    struct stat file_stat;
    fstat(in_fd, &file_stat);
    size_t file_size = file_stat.st_size;
    int N = 1;
    while ((unsigned) N * N < file_size / sizeof(double)) {
        ++N;
    }
    ftruncate(out_fd, N * N * sizeof(double));
    double *in_mem = mmap(NULL, file_size,
            PROT_READ, MAP_SHARED, in_fd, 0);
    close(in_fd);
    double *out_mem = mmap(NULL, file_size,
            PROT_WRITE, MAP_SHARED, out_fd, 0);
    close(out_fd);

    if (P > N) {
        P = larger2power(N);
    }


    double *cur_mat = mmap(NULL, file_size,
            PROT_WRITE | PROT_READ, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    double *cur_mul_mat = mmap(NULL, file_size,
            PROT_WRITE | PROT_READ, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    double *cur_mat_tmp = mmap(NULL, file_size,
            PROT_WRITE | PROT_READ, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    double *cur_mul_mat_tmp = mmap(NULL, file_size,
            PROT_WRITE | PROT_READ, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    // Init cur_mul_mat
    memcpy(cur_mul_mat, in_mem, file_size);
    // Init cur_mat
    for (int i = 0; i < N - 1; ++i) {
        for (int j = i + 1; j < N; ++j) {
            cur_mat[i * N + j] = cur_mat[j * N + i] = +INFINITY;
        }
    }
    for (int i = 0; i < N; ++i) {
        cur_mat[i * N + i] = 0;
    }

    while (P) {
        if (P % 2) {
            MatrixMul(cur_mat_tmp, cur_mat, cur_mul_mat, N);
            // Swap
            double *tmp = cur_mat_tmp;
            cur_mat_tmp = cur_mat;
            cur_mat = tmp;
            --P;
        } else {
            MatrixMul(cur_mul_mat_tmp, cur_mul_mat, cur_mul_mat, N);
            // Swap
            double *tmp = cur_mul_mat_tmp;
            cur_mul_mat_tmp = cur_mul_mat;
            cur_mul_mat = tmp;
            P /= 2;
        }
    }
    memcpy(out_mem, cur_mat, file_size);
    munmap(cur_mat, file_size);
    munmap(cur_mat_tmp, file_size);
    munmap(cur_mul_mat, file_size);
    munmap(cur_mul_mat_tmp, file_size);
    
    munmap(in_mem, file_size);
    munmap(out_mem, file_size);
    return 0;
}
       
int
larger2power(int x)
{
    int tmp = 1;
    while (tmp < x) {
        tmp <<= 1;
    }
    return tmp;
}

void
MatrixMul(double *mat_dest, double *mat1, double *mat2, int N)
{
    // Put zeroes on the main diagonal
    for (int i = 0; i < N; ++i) {
        mat_dest[i * N + i] = 0.0;
    }

    // Calculate all other values, given that the matrices are symmetrical
    for (int i = 0; i < N - 1; ++i) {
        for (int j = i + 1; j < N; ++j) {
            double min_val = +INFINITY;
            for (int s = 0; s < N; ++s) {
                double sum = mat1[i * N + s] + mat2[j * N + s];
                if (sum < min_val) {
                    min_val = sum;
                }
            }
            mat_dest[i * N + j] = mat_dest[j * N + i] = min_val;
        }
    }
}
