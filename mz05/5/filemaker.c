#include <stdio.h>
#include <stdint.h>

struct Data
{
    int16_t x;
    int64_t y;
};

typedef unsigned char uchar;

void
write_data(const struct Data *data, uchar *cont_base)
{
    // warning: magic number again
    uint16_t ux = data->x;
    uint64_t uy = data->y;

    uint8_t mask = 0xffu;

    cont_base[0] = ux & mask;
    cont_base[1] = (ux >> 8) & mask;

    cont_base[2 + 0] = uy & mask;
    cont_base[2 + 1] = (uy >> 8) & mask;
    cont_base[2 + 2] = (uy >> 16) & mask;
    cont_base[2 + 3] = (uy >> 24) & mask;
    cont_base[2 + 4] = (uy >> 32) & mask;
    cont_base[2 + 5] = (uy >> 40) & mask;
    cont_base[2 + 6] = (uy >> 48) & mask;
    cont_base[2 + 7] = (uy >> 56) & mask;
}

int
main(void)
{
    FILE *out = fopen("file", "w+");
    printf("Print structure fields in pairs: x y\n");
    int16_t x;
    int64_t y;
    uchar cont_base[10240];
    size_t str_count = 0;
    while (scanf("%hd %lld", &x, &y) == 2) {
        struct Data data;
        data.x = x;
        data.y = y;
        write_data(&data, cont_base + str_count++ * 10);
    }
    fwrite(cont_base, 10, str_count, out);
    fclose(out);
}
