#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>

mysys(const char *str)
{
    pid_t child_pid = fork();
    if (child_pid == 0) {
        buff[8192];
        snprintf(buff, "sh -c %s", str);
        execlp( 
