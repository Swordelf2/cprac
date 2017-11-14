#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int
main(int argc, char *argv[])
{
    // fd[i] is where i-th process writes to
    int n = strtol(argv[1], 0, 10);
    int fd[2][2];
    pipe(fd[0]);
    pipe(fd[1]);
    
    for (int i = 0; i < 2; ++i) {
        if (fork() == 0) {
            close(fd[1-i][1]);
            close(fd[i][0]);
            FILE *p_in = fdopen(fd[1-i][0], "r");
            FILE *p_out = fdopen(fd[i][1], "w");
            if (i == 1) {
                fprintf(p_out, "1 ");
                fflush(p_out);
            }
            int a;
            while (fscanf(p_in, "%d", &a) != EOF) {
                if (a == n) {
                    break;
                }
                printf("%d %d\n", i + 1, a);
                fflush(stdout);
                if (++a >= n) {
                    break;
                }
                fprintf(p_out, "%d ", a);
                fflush(p_out);
            }
            fclose(p_in);
            fclose(p_out);
            exit(0);
        }
    }

    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            close(fd[i][j]);
        }
    }
    for (int i = 0; i < 2; ++i) {
        wait(NULL);
    }
    printf("Done\n");
    return 0;
}

