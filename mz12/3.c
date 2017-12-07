#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <sys/sem.h>

#include <stdlib.h>
#include <stdio.h>

// Returns x % y (mathematical)
int
rem(int x, int y)
{
    int rem = x % y;
    if (rem < 0) {
        return y + rem;
    } else {
        return rem;
    }
}

int
main(int argc, char *argv[])
{
    int count = strtol(argv[1], NULL, 0);
    setbuf(stdin, NULL);
    key_t key = ftok(argv[0], 0);

    int semid = semget(key, count, 0600 | IPC_CREAT);

    for (int i = 0; i < count; ++i) {
        semctl(semid, i, SETVAL, 0);
    }
    for (int p = 0; p < count; ++p) {
        if (fork() == 0) {
            // Down(p)
            if ((semop(semid, 
                    (struct sembuf[]) {{.sem_num = p, .sem_op = -1}},
                    1)) == -1) {
                _exit(0);
            }

            int x;
            int sc_res;
            sc_res = scanf("%d", &x);
            if (sc_res != 1) {
                // Abort
                semctl(semid, 0, IPC_RMID);
                _exit(0);
            }
            printf("%d %d\n", p, x);
            fflush(stdout);

            //Up(x mod P)
            semop(semid,
                    (struct sembuf[]) {{.sem_num = rem(x, count), .sem_op = +1}},
                    1);
        }
    }

    semop(semid, (struct sembuf[]) {{.sem_num = 0, .sem_op = +1}}, 1);

    while (wait(NULL) > 0);
                               
    return 0;
}
