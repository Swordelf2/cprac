#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int
exec_iter(const char *cmd);


int
main(int argc, char *argv[])
{
    long long gcount = 0;
    int g1 = 0;
    int g2 = 0;
    while ((g1 = exec_iter(argv[1])) || (g2 = exec_iter(argv[2]))) {
        gcount += g1 + g2;
        g1 = g2 = 0;
        // Parellel
        for (int i = 3; i < argc; ++i) {
            if (fork() == 0) {
                execlp(argv[i], argv[i], NULL);
                _exit(1);
            }
        }
        
        int status;
        while (wait(&status) > 0) {
            if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
                ++gcount;
            }
        }
    }

    printf("%lld\n", gcount);
    return 0;
}

int
exec_iter(const char *cmd)
{
    pid_t pid;
    if ((pid = fork()) == 0) {
        execlp(cmd, cmd, NULL);
        _exit(1);
    }

    int status;
    waitpid(pid, &status, 0);
    return WIFEXITED(status) && (WEXITSTATUS(status) == 0);
}
