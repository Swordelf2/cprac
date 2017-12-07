#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include <unistd.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>

void
operation(int *data, int ind1, int ind2, int val);

int
main(int argc, char *argv[])
{
    int count = strtol(argv[1], NULL, 0);
    int key = strtol(argv[2], NULL, 0);
    int nproc = strtol(argv[3], NULL, 0);
    int iter_count = strtol(argv[4], NULL, 0);

    volatile int *shared = mmap(NULL, count * sizeof(*shared), 
            PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0);

    for (int i = 0; i < count; ++i) {
        scanf("%d", &shared[i]);
    }
    int semid = semget(key, count, 0600 | IPC_CREAT);


    for (int i = 0; i < count; ++i) {
        semctl(semid, i, SETVAL, 1);
    }
    for (int p = 0; p < nproc; ++p) {
        if (fork() == 0) {
            srand(strtoul(argv[5 + p], NULL, 0));
            for (int i = 0; i < iter_count; ++i) {
                int ind1, ind2;
                ind1 = rand() % count;
                ind2 = rand() % count;
                int val = rand() % 10;
                if (ind1 != ind2) {
                    semop(semid, (struct sembuf[]) {{.sem_num = ind1, .sem_op = -1},
                            {.sem_num = ind2, .sem_op = -1}}, 2);
                    operation((void *) shared, ind1, ind2, val);
                    semop(semid, (struct sembuf[]) {{.sem_num = ind1, .sem_op = +1},
                            {.sem_num = ind2, .sem_op = +1}}, 2);
                }
            }
            _exit(0);
        }
    }
    while (wait(NULL) > 0);
    // Print
    for (int i = 0; i < count; ++i) {
        printf("%d ", shared[i]);
    }
    putchar('\n');

    munmap((void *) shared, count * sizeof(*shared));
    semctl(semid, 0, IPC_RMID);
    return 0;
}
