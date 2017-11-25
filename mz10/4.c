#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int
start_execution(const char *cmd, int fd_in, int fd_out);

int
main(int argc, char *argv[])
{
    int fail_flag = 0;
    int p[2], prev_p[2], fd_in_toclose = -1;
    // p is the right pipe, prev_p is the left pipe of the current command
    // fd_in_toclose is the pipe to the left of the previous command
    for (int i = 1; i < argc; ++i) {
        if (fd_in_toclose != -1) {
            close(fd_in_toclose);
        }
        if (i == 1) {
            prev_p[0] = 0;
        } else {
            close(prev_p[1]);
            fd_in_toclose = prev_p[0];
        }
        if (i == argc - 1) {
            p[1] = 1;
        } else {
            if (pipe(p) == -1) {
                fail_flag = 1;
                goto main_exit;
            }
        }
        if (!start_execution(argv[i], prev_p[0], p[1])) {
            fail_flag = 1;
            goto main_exit;
        }
        memcpy(prev_p, p, sizeof(prev_p));
    }
    if (fd_in_toclose != -1) {
        close(fd_in_toclose);
    }

main_exit:
    if (fail_flag) {
        kill(0, SIGKILL);
    }
    while (wait(NULL) > 0);
    return fail_flag;
}

int
start_execution(const char *cmd, int fd_in, int fd_out)
{
    pid_t pid = fork();
    if (pid == -1) {
        return 0;
    } else if (pid == 0) {
        dup2(fd_in, 0);
        close(fd_in);
        dup2(fd_out, 1);
        close(fd_out);
        execlp(cmd, cmd, NULL);
        _exit(-1);
    }
    return 1;
}
