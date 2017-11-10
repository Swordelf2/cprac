#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

enum
{
    ARG_POS_FILENAME = 1,
    ARG_POS_LINE_ONE = 2,
    ARG_POS_LINE_TWO = 3,
    ARG_COUNT = 1 + 3,
    DEC_BASE = 10
};

void
print_error(const char *error_str);

int
main(int argc, char *argv[])
{
    int fail_flag = 0;
    if (argc != ARG_COUNT) {
        fail_flag = 1;
        fputs("Invalid number of arguments\n", stderr);
        goto exit;
    }
    // Init line1 and line2 variables
    long line1, line2;
    char *end_ptr;
    errno = 0;
    line1 = strtol(argv[ARG_POS_LINE_ONE], &end_ptr, DEC_BASE);
    if (errno != 0) {
        fail_flag = 1;
        print_error("Invalid line1 argument");
        goto exit;
    }
    if (line1 <= 0) {
        fail_flag = 1;
        fputs("Invalid line1 argument: Non-positive value\n", stderr);
        goto exit;
    }
    if (*argv[ARG_POS_LINE_ONE] == '\0' || (*end_ptr != '\0' && *end_ptr != ' ')) { 
        fail_flag = 1;
        fputs("Invalid line1 argument\n", stderr);
        goto exit;
    }
    errno = 0;
    line2 = strtol(argv[ARG_POS_LINE_TWO], &end_ptr, DEC_BASE);
    if (errno != 0) {
        fail_flag = 1;
        print_error("Invalid line2 argument");
        goto exit;
    }
    if (line2 <= 0) {
        fail_flag = 1;
        fputs("Invalid line2 argument: Non-positive value\n", stderr);
        goto exit;
    }
    if (*argv[ARG_POS_LINE_TWO] == '\0' || (*end_ptr != '\0' && *end_ptr != ' ')) {
        fail_flag = 1;
        fputs("Invalid line2 argument\n", stderr);
        goto exit;
    }
    // Simple check
    if (line1 >= line2) {
        goto exit;
    }
    // Open file
    int fd = open(argv[ARG_POS_FILENAME], O_RDONLY);
    if (fd == -1) {
        fail_flag = 1;
        print_error("Could not open file");
        goto exit;
    }
    // Retrieve file size
    off_t file_size = lseek(fd, 0, SEEK_END);
    if ((unsigned long long) file_size > SIZE_MAX) {
        fail_flag = 1;
        print_error("File size is too large");
        goto cleanup_file;
    }
    // Map the file
    char *base = mmap(NULL, file_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (base == MAP_FAILED) {
        fail_flag = 1;
        print_error("Could not map the file");
        goto cleanup_file;
    }

    // Skip first (line1 - 1) lines
    char *ptr = base;
    for (long i = 0; i < line1 - 1 && ptr - base < file_size; ++i) {
        while (*ptr != '\n' && ptr - base < file_size) {
            ++ptr;
        }
        ++ptr;
    }

    // Store pointers to next (line2 - line1) lines
    char **ptr_arr = malloc((line2 - line1) * sizeof(*ptr_arr));
    long i;
    for (i = 0; i < line2 - line1 && ptr - base < file_size; ++i) {
        ptr_arr[i] = ptr;
        while (*ptr != '\n' && ptr - base < file_size) {
            ++ptr;
        }
        ++ptr;
    }
    long arr_size = i;

    // Print all lines in reverse order
    for (i = arr_size - 1; i >= 0; --i) {
        ptr = ptr_arr[i];
        do {
            putchar(*ptr);
            ++ptr;
        } while (ptr[-1] != '\n');
    }
    free(ptr_arr);

    munmap(base, file_size);
cleanup_file:
    close(fd);
exit:
    return fail_flag;
    // skip first (line1 - 1) 
}

void
print_error(const char *error_str)
{
    fprintf(stderr, "%s: %s\n", error_str, strerror(errno));
}
