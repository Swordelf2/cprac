#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

mysys(const char *str)
{
    size_t arg_count;
    char *agr_val[1024];
    for (arg_count = 0; ;++arg_count) {
        prev = str;
        while (*str != ' ' && *str != '\0') {
            ++str;
        }
        arg_val[arg_count] = strndup(prev, str - prev);
        if (*str == '\0') {
            break;
        }
    }

    pid_t child_pid = fork();
    if (child_pid == 0) {
        execlp("sh", "sh", "-c", 
