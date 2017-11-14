#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <time.h>


int
main(void)
{
    int fd[2];
    pipe(fd);
    time_t cur_time = time(NULL);
    for (int i = 0; i < 3; ++i) {
        write(fd[1], &cur_time, sizeof(cur_time));
    }
    close(fd[1]);
    if (fork() == 0) {
        if (fork() == 0) {
            if (fork() == 0) {
                time_t s_time;
                read(fd[0], &s_time, sizeof(s_time));
                close(fd[0]);
                struct tm *s_tm = localtime(&s_time);
                printf("S:%02d\n", s_tm->tm_sec);
                fflush(stdout);
            } else {
                wait(NULL);
                time_t m_time;
                read(fd[0], &m_time, sizeof(m_time));
                close(fd[0]);
                struct tm *m_tm = localtime(&m_time);
                printf("M:%02d\n", m_tm->tm_min);
                fflush(stdout);
            } 
        } else {
            wait(NULL);
            time_t h_time;
            read(fd[0], &h_time, sizeof(h_time));
            close(fd[0]);
            struct tm *h_tm = localtime(&h_time);
            printf("H:%02d\n", h_tm->tm_hour);
            fflush(stdout);
        }
    } else {
        wait(NULL);
        close(fd[0]);
    }
    return 0;
}
