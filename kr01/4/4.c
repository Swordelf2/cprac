#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>

#include <string.h>
#include <stdlib.h>

// resizable string
typedef struct String
{
    char *str;
    size_t max_size;
} String;

enum
{
    FNAME_ARG_POS = 1,
    STR_INIT_SIZE = 16,
    STR_EXTEND_MUL = 2,
    LEN_FIELD_SIZE = 2,
    OFF_FIELD_SIZE = 4
};

int
string_init(String *string);

int
string_extend(String *string, size_t new_size);
// Extend the sring at least up to given size

int
string_read(String *string, int fd, size_t len);
// Reads a string of length len from file descriptor fd

int
string_compare(String *string1, String *string2);

int
string_print(String *string, int out_fd);

void
string_free(String *string);

void
safe_exit(String *string1, String *string2, int fd);


int
main(int argc, char *argv[])
{
    //TODO: don't do anything if the file is empty
    if (argc < 2) {
        return -1;
    }
    
    int fd = open(argv[FNAME_ARG_POS], O_RDONLY);
    if (fd == -1) {
        return -1;
    }
    
    off_t cur_offset = 0;

    String cur_string, max_string;
    int max_string_init = 0;
    if (!string_init(&cur_string) || !string_init(&max_string)) {
        safe_exit(&cur_string, &max_string, fd);
        return -1;
    }
        
    while (1) {
        // Read len and offset
        char buf[LEN_FIELD_SIZE + OFF_FIELD_SIZE];
        size_t br_count = 0;
        while (br_count < sizeof(buf)) {
            ssize_t br_add = read(fd, buf + br_count, sizeof(buf) - br_count);
            // note that 0 here may indicate that the whoe file is empty,+
            // which is not an error
            if (br_add <= 0) {
                safe_exit(&cur_string, &max_string, fd);
                return 0;
            }
            br_count += br_add;
        }
        // Now read the string
        size_t len = * (uint16_t *) buf;
        if (!string_read(&cur_string, fd, len)) {
            safe_exit(&cur_string, &max_string, fd);
            return -1;
        }
        // Compare current string to max_string
        if (!max_string_init || (string_compare(&cur_string, &max_string) > 0)) {
            max_string_init = 1;
            // Swap max_string and cur_string
            String temp = max_string;
            max_string = cur_string;
            cur_string = temp;
        }
        cur_offset = * (int32_t *) (buf + LEN_FIELD_SIZE);
        if (cur_offset == 0) {
            break;
        } else {
            lseek(fd, - LEN_FIELD_SIZE - OFF_FIELD_SIZE - len + cur_offset, SEEK_CUR);
        }
    }

    if (!string_print(&max_string, 1)) {
        safe_exit(&max_string, &cur_string, fd);
        return -1;
    }
    safe_exit(&max_string, &cur_string, fd);
    return 0;
}

int
string_init(String *string)
{
    string->max_size = STR_INIT_SIZE;
    string->str = malloc(string->max_size);
    if (!string->str) {
        return 0;
    }
    return 1;
}

int
string_extend(String *string, size_t new_size)
{
    if (string->max_size >= new_size) {
        return 1;
    }
    while (string->max_size < new_size) {
        string->max_size *= STR_EXTEND_MUL;
    }
    char *new_ptr = realloc(string->str, string->max_size);
    if (!new_ptr) {
        return 0;
    }
    string->str = new_ptr;
    return 1;
}

int
string_read(String *string, int fd, size_t len)
{
    if (!string_extend(string, len + 1)) {
        return 0;
    }
    size_t br_count = 0;
    while (br_count < len) {
        ssize_t br_add = read(fd, string->str + br_count, len - br_count);
        // Note that in both cases 0 and -1 this is an error for the given task
        if (br_add <= 0) {
            return 0;
        }
        br_count += br_add;
    }
    string->str[len] = '\0';
    return 1;
}

int
string_compare(String *string1, String *string2)
{
    return strcmp(string1->str, string2->str);
}

int
string_print(String *string, int out_fd)
{
    size_t wb_count = 0;
    size_t len = strlen(string->str);
    while (wb_count < len) {
        ssize_t wb_add = write(out_fd, string->str + wb_count, len - wb_count);
        if (wb_add == -1) {
            return 0;
        }
        wb_count += wb_add;
    }
    write(out_fd, "\n", 1);
    return 1;
}

void
string_free(String *string)
{
    free(string->str);
}

void
safe_exit(String *string1, String *string2, int fd)
{
    if (string1) {
        string_free(string1);
    }
    if (string2) {
        string_free(string2);
    }
    close(fd);
}
