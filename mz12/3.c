#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <sys/sem.h>

#include <stdlib.h>
#include <stdio.h>

int
main(int argc, char *argv[])
{
    int count = strtol(argv[1], NULL, 0);
    setbuf(stdout, NULL);
    key_t key = ftok(argv[0], 0);

    int semid = semget(key, count, 0600 | IPC_CREAT);

    for (int i = 0; i < count; ++i) {
        semctl(semid, i, SETVAL, 0);
    }
    for (int p = 0; p < count; ++p) {
        if (fork() == 0) {
            semop(semid, (struct sembuf[]) {
                    {

    return 0;
}
