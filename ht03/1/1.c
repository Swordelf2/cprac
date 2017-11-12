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

void
print_line(const char *line_start);

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
    if (*argv[ARG_POS_LINE_ONE] == '\0' || *end_ptr != '\0') { 
        fail_flag = 1;
        fputs("Invalid line1 argument\n", stderr);
        goto exit;
    }
    if (line1 <= 0) {
        fail_flag = 1;
        fputs("Invalid line1 argument: Non-positive value\n", stderr);
        goto exit;
    }
    errno = 0;
    line2 = strtol(argv[ARG_POS_LINE_TWO], &end_ptr, DEC_BASE);
    if (errno != 0) {
        fail_flag = 1;
        print_error("Invalid line2 argument");
        goto exit;
    }
    if (*argv[ARG_POS_LINE_TWO] == '\0' || *end_ptr != '\0') {
        fail_flag = 1;
        fputs("Invalid line2 argument\n", stderr);
        goto exit;
    }
    if (line2 <= 0) {
        fail_flag = 1;
        fputs("Invalid line2 argument: Non-positive value\n", stderr);
        goto exit;
    }
    // Open file
    int fd = open(argv[ARG_POS_FILENAME], O_RDONLY);
    if (fd == -1) {
        fail_flag = 1;
        print_error("Could not open file");
        goto exit;
    }
    struct stat file_stat;
    if (fstat(fd, &file_stat) == -1) {
        fail_flag = 1;
        print_error("Could not retrive file stat");
        goto cleanup_file;
    }
    if (!S_ISREG(file_stat.st_mode)) {
        fail_flag = 1;
        fputs("The file is not regular\n", stderr);
        goto cleanup_file;
    }
    // Retrieve file size
    off_t file_size = file_stat.st_size;
    if ((unsigned long long) file_size > SIZE_MAX) {
        fail_flag = 1;
        fputs("The file is too large", stderr);
        goto cleanup_file;
    }
    if (file_size == 0) {
        // Do nothing
        goto cleanup_file;
    }
    // Map the file
    char *base = mmap(NULL, file_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (base == MAP_FAILED) {
        fail_flag = 1;
        print_error("Could not map the file");
        goto cleanup_file;
    }

    // Shift line1 and line2 so that they become 0-indexed
    --line1; --line2;
    // Skip first line2 lines
    char *ptr = base;
    long i;
    for (i = 0; i < line2 && ptr - base < file_size; ++i) {
        while (*ptr != '\n') {
            if (ptr - base >= file_size) {
                fail_flag = 1;
                fputs("Last line doesn't end with a newline character\n", stderr);
                goto cleanup_file;
            }
            ++ptr;
        }
        ++ptr;
    }
    // now i == lines skipped
    long lines_skipped = i;
    // In a loop we move to the previous line and print it
    for (i = lines_skipped - 1; i >= line1; --i) {
        --ptr;
        while (ptr != base && ptr[-1] != '\n') {
            --ptr;
        }
        print_line(ptr);
    }

    if (munmap(base, file_size) == -1) {
        fail_flag = 1;
        print_error("Could not unmap file");
        goto cleanup_file;
    }
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

void
print_line(const char *line_start)
{
    const char *ptr = line_start;
    do {
        putchar(*ptr);
        ++ptr;
    } while (ptr[-1] != '\n');
}
