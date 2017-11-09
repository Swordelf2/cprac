#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <stdint.h>
#include <stdio.h>
#include <errno.h>

enum
{
    ARG_POS_FILENAME = 1,
    ARG_POS_LINE1 = 2,
    ARG_POS_LINE2 = 3
};

void
print_error(const char *error_str);

int
main(int argc, char *argv[])
{
    int fail_flag = 0;
    // Init line1 and line2 variables
    long line1, line2;
    errno = 0;
    line1 = strtol(argv[ARG_POS_LINE1], NULL, 10);
    if (errno != 0) {
        fail_flag = 1;
        print_error("Invalid line1 argument");
        goto exit;
    }
    if (line1 <= 0) {
        fail_flag = 1;
        fputs("Invalid line1 argument: Non-positive value", stderr);
        goto exit;
    }
    errno = 0;
    line2 = strtol(argv[ARG_POS_LINE2], NULL, 10);
    if (errno != 0) {
        fail_flag = 1;
        print_error("Invalid line2 argument");
        goto exit;
    }
    if (line2 <= 0) {
        fail_flag = 1;
        fputs("Invalid line2 argument: Non-positive value", stderr);
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
    if (file_size > SIZET_MAX) {
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
    for (long i = 0; i < line1; ++i) [
        while (*ptr != '\n' && ptr - base < file_size) 

cleanup_map:
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
