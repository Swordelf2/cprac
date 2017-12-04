#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

volatile int flag;

enum ArgPos
{
    AP_X = 1,
    AP_DX,
    AP_P,
    AP_N,
};

enum
{
    EXPECTED_ARG_COUNT = 4,
    DEC_BASE = 10,
    MAX_OUTSTR_SIZE = 64
};
    
void
handler(int sig)
{
    flag = 1;
}

int
main(int argc, char *argv[])
{
    if (argc < 1 + EXPECTED_ARG_COUNT) {
        fprintf(stderr, "Specify %d arguments\n", EXPECTED_ARG_COUNT);
        exit(1);
    }
    double x = strtod(argv[AP_X], NULL); 
    double dx = strtod(argv[AP_DX], NULL);
    int P = strtol(argv[AP_P], NULL, DEC_BASE);
    int N = strtol(argv[AP_N], NULL, DEC_BASE);

    sigset_t block_set, dfl_set;
    sigemptyset(&block_set);
    sigaddset(&block_set, SIGUSR1);
    sigprocmask(SIG_BLOCK, &block_set, &dfl_set);
    sigaction(SIGUSR1, &(struct sigaction) {.sa_handler = handler}, NULL);

    int fd[2]; // pipe to give every process the pid of the next child
    pipe(fd);
    FILE *p_out = fdopen(fd[1], "w");
    pid_t first_pid;
    for (int p = 0; p < P; ++p) {
        pid_t pid = fork();
        if (pid == 0) {
            // Child
            // Wait for signal
            while (!flag) {
                sigsuspend(&dfl_set);
            }
            flag = 0;
            // Read the pid of the next process in the list
            pid_t next_pid;
            read(fd[0], &next_pid, sizeof(next_pid));
            // Send signal to the next process
            kill(next_pid, SIGUSR1);
            close(fd[0]);
            for (int i = 0; i < N; ++i) {
                // Calculate the sin
                char out_str[MAX_OUTSTR_SIZE];
                // TODO: insert actual sin
                snprintf(out_str, sizeof(out_str), "p: %d; i: %d; n: %d\n", p, i, i * P + p);
                while (!flag) {
                    sigsuspend(&dfl_set);
                }
                flag = 0;
                fputs(out_str, stdout); fflush(stdout);
                kill(next_pid, SIGUSR2);
            }
            _exit(0);
        }

        // Parent
        if (p == 0) {
            first_pid = pid;
        } else {
            // Write the pid of this child into the pipe
            fwrite(&pid, sizeof(pid), 1, p_out);
        }
    }
    close(fd[0]);
    // Finally write the pid of the first_child
    fwrite(&first_pid, sizeof(first_pid), 1, p_out);
    fclose(p_out);
    kill(first_pid, SIGUSR1);

    while (wait(NULL) > 0);

    return 0;
}
