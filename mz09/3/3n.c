#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

int
mysys(const char *str)
{
    pid_t child_pid = fork();
    if (child_pid == 0) {
        execlp("sh", "sh", "-c", str, NULL);
        return -1;
    } else if (child_pid == -1) {
        return -1;
    } else {
        int status;
        waitpid(child_pid, &status, 0);
        if (WIFEXITED(status)) {
            int ex_status = WEXITSTATUS(status);
            if (ex_status >= 0 && ex_status <= 127) {
                return ex_status;
            } else
                return -1;
        } else if (WIFSIGNALED(status)) {
            return 128 + WTERMSIG(status);
        } else {
            return -1;
        }
    }
}
