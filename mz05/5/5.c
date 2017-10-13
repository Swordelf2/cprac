#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>

#include <stdint.h>
#include <stdio.h> //TODO: remove this
#include <string.h> // TODO: remove this

typedef unsigned char uchar;

struct Data
{
    int16_t x;
    int64_t y;
};

void
read_data(struct Data *data, uchar *base);

int
modify_data(struct Data *data, int32_t a);

void
write_data(const struct Data *data, uchar *base);

int
main(int argc, char *argv[])
{
    if (argc <= 1) {
        printf("Specify a filename\n");
        return 1;
    }
    int fd = open(argv[1], O_RDWR);
    size_t file_size = lseek(fd, 0, SEEK_END);
    uchar *file_cont = mmap(NULL, file_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    struct Data data;
    read_data(&data, file_cont);
    
    munmap(file_cont, file_size);

    return 0;
}

void
read_data(struct Data *data, uchar *cont_base)
{
    // warning: magic numbers
    uint16_t ux = (uint16_t) cont_base[0] + ((uint16_t) cont_base[2] << 8);
    uint64_t uy = (uint64_t) cont_base[2 + 0] +
            ((uint64_t) cont_base[2 + 1] << 8) +
            ((uint64_t) cont_base[2 + 2] << 16) +
            ((uint64_t) cont_base[2 + 3] << 24) +
            ((uint64_t) cont_base[2 + 4] << 32) +
            ((uint64_t) cont_base[2 + 5] << 40) +
            ((uint64_t) cont_base[2 + 6] << 48) +
            ((uint64_t) cont_base[2 + 7] << 56);
    data->x = ux;
    data->y = uy;
}
