#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

enum
{
    BYTE_ONE_SHIFT = 8,
    BYTE_TWO_SHIFT = 16,
    BYTE_THREE_SHIFT = 24,
    BYTE_FOUR_SHIFT = 32,
    BYTE_FIVE_SHIFT = 40,
    BYTE_SIX_SHIFT = 48,
    BYTE_SEVEN_SHIFT = 56
};

typedef unsigned char uchar;

struct Data
{
    int16_t x;
    int64_t y;
};

enum
{
    STRUCT_SIZE = 10
};

void
read_data(struct Data *data, uchar *cont_base);

int
modify_data(struct Data *data, int32_t A);

void
write_data(const struct Data *data, uchar *cont_base);

int
main(int argc, char *argv[])
{
    if (argc <= 2) {
        printf("Specify a filename and an integer\n");
        return 1;
    }

    int fd = open(argv[1], O_RDWR);
    size_t file_size = lseek(fd, 0, SEEK_END);
    uchar *file_cont = mmap(NULL, file_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    int A = atoi(argv[2]);

    int size_odd = (file_size / STRUCT_SIZE) % 2;
    struct Data data1, data2;
    for (size_t i = 0; i < (file_size / STRUCT_SIZE) / 2; ++i) {
        uchar *pos1 = file_cont + i * STRUCT_SIZE;
        uchar *pos2 = file_cont + file_size - (i + 1) * STRUCT_SIZE;
        read_data(&data1, pos1);
        read_data(&data2, pos2);
        if (!modify_data(&data1, A) || !modify_data(&data2, A)) {
            fprintf(stderr, "Error: Arithmetic overflow\n");
            return 3;
        }
        write_data(&data1, pos2);
        write_data(&data2, pos1);
    }
    if (size_odd) {
        uchar *pos = file_cont + (file_size / STRUCT_SIZE) / 2 * STRUCT_SIZE;
        read_data(&data1, pos);
        if (!modify_data(&data1, A)) {
            fprintf(stderr, "Error: Arithmetic overflow\n");
            return 3;
        }
        write_data(&data1, pos);
    }

    munmap(file_cont, file_size);

    return 0;
}

void
read_data(struct Data *data, uchar *cont_base)
{
    uint16_t ux = (uint16_t) cont_base[0] + ((uint16_t) cont_base[1] << BYTE_ONE_SHIFT);
    uint64_t uy = (uint64_t) cont_base[2 + 0] +
            ((uint64_t) cont_base[2 + 1] << BYTE_ONE_SHIFT) +
            ((uint64_t) cont_base[2 + 2] << BYTE_TWO_SHIFT) +
            ((uint64_t) cont_base[2 + 3] << BYTE_THREE_SHIFT) +
            ((uint64_t) cont_base[2 + 4] << BYTE_FOUR_SHIFT) +
            ((uint64_t) cont_base[2 + 5] << BYTE_FIVE_SHIFT) +
            ((uint64_t) cont_base[2 + 6] << BYTE_SIX_SHIFT) +
            ((uint64_t) cont_base[2 + 7] << BYTE_SEVEN_SHIFT);
    data->x = ux;
    data->y = uy;
}

int
modify_data(struct Data *data, int32_t A)
{
    int64_t prod = (int64_t) data->x * A;
    int64_t y = data->y;
    if (__builtin_add_overflow(y, prod, &data->y)) {
        return 0;
    }
    return 1;
}

void
write_data(const struct Data *data, uchar *cont_base)
{
    uint16_t ux = data->x;
    uint64_t uy = data->y;

    uint8_t mask = 0xffu;

    cont_base[0] = ux & mask;
    cont_base[1] = (ux >> BYTE_ONE_SHIFT) & mask;

    cont_base[2 + 0] = uy & mask;
    cont_base[2 + 1] = (uy >> BYTE_ONE_SHIFT) & mask;
    cont_base[2 + 2] = (uy >> BYTE_TWO_SHIFT) & mask;
    cont_base[2 + 3] = (uy >> BYTE_THREE_SHIFT) & mask;
    cont_base[2 + 4] = (uy >> BYTE_FOUR_SHIFT) & mask;
    cont_base[2 + 5] = (uy >> BYTE_FIVE_SHIFT) & mask;
    cont_base[2 + 6] = (uy >> BYTE_SIX_SHIFT) & mask;
    cont_base[2 + 7] = (uy >> BYTE_SEVEN_SHIFT) & mask;
}
