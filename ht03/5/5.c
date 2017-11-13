#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>

#include <sys/mman.h>
#include <sys/wait.h>

enum
{
    DEC_BASE = 10,
    PROC_COUNT = 2,
    BLOCK_COUNT = 2,
    PROC_TO_RUN_IND = 0,
    NEXT_NUMBER_IND = 1,
};

int
main(int argc, char *argv[])
{
    int N = strtol(argv[1], NULL, DEC_BASE);

    volatile int *shared_mem = mmap(NULL, 
            BLOCK_COUNT * sizeof(int),
            PROT_READ | PROT_WRITE,
            MAP_SHARED | MAP_ANONYMOUS,
            -1,
            0);
    shared_mem[PROC_TO_RUN_IND] = 1; // process 1 will start first
    shared_mem[NEXT_NUMBER_IND] = 1;
    int proc_id;
    for (proc_id = 1; proc_id <= PROC_COUNT; ++proc_id) {
        if (fork() == 0) {
            int exit_flag = 0;
            while (!exit_flag) {
                if (shared_mem[PROC_TO_RUN_IND] == proc_id) {
                    if (shared_mem[NEXT_NUMBER_IND] <= N) {
                        // Print the proc id and the number
                        printf("%d %d\n", proc_id, shared_mem[NEXT_NUMBER_IND]);
                        fflush(stdout);
                        // Increment the shared number
                        ++shared_mem[NEXT_NUMBER_IND];
                    } else {
                        exit_flag = 1;
                    }
                    // Give the control to the other process
                    if (proc_id == 1) {
                        shared_mem[PROC_TO_RUN_IND] = 2;
                    } else {
                        shared_mem[PROC_TO_RUN_IND] = 1;
                    }
                    sched_yield();
                }
            }
            exit(0);
        }
    }
    wait(NULL);
    wait(NULL);
    munmap((void *) shared_mem, 1);
    return 0;
}
