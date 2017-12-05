#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include <unistd.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/ipc.h>
#include <sys/sem.h>

int
main(int argc, char *argv[])
{
    int nproc = strtol(argv[1], NULL, 0);
    int key = strtol(argv[2], NULL, 0);
    int maxval = strtol(argv[3], NULL, 0);

    int semid = semget(key, nproc, 0600 | IPC_CREAT);

    volatile int *shared = mmap(NULL, sizeof(*shared), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0);
    *shared = 0;

    for (int p = 1; p < nproc; ++p) {
        semctl(semid, p, SETVAL, 0);
    }
    semctl(semid, 0, SETVAL, 1);

    for (int p = 0; p < nproc; ++p) {
        if (fork() == 0) {
            while (1) {
                semop(semid, (struct sembuf[]) {{.sem_num = p, .sem_op = -1}}, 1);
                int val = *shared;
                if (val == -1) {
                    _exit(0);
                }
                printf("%d: %d\n", p, val); fflush(stdout);
                if (val >= maxval) {
                    *shared = -1;
                    // Up for everyone
                    for (int i = 0; i < nproc; ++i) {
                        if (i != p) {
                            semop(semid, (struct sembuf[]) {{.sem_num = i, .sem_op = +1}}, 1);
                        }
                    }
                    _exit(0);
                }
                ++val;
                *shared = val;
                semop(semid, (struct sembuf[]) {{.sem_num = (val * val) % nproc, .sem_op = +1}}, 1);
            }
        }
    }

    while (wait(NULL) > 0);

    munmap((void *) shared, sizeof(*shared));
    semctl(semid, 0, IPC_RMID);
    return 0;
}
