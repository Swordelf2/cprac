#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

int
main(int argc, char *argv[])
{
    int port = strtol(argv[1], NULL, 0);
    int list_fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in myaddr =
    {
        .sin_family = AF_INET,
        .sin_port = htons(port),
        .sin_addr.s_addr = INADDR_ANY
    };
    bind(list_fd, (struct sockaddr *) &myaddr, sizeof(myaddr));

    listen(list_fd, 0);
    int client_fd;
    long long sum = 0;
    while (1) {
        client_fd = accept(listenfd, NULL, NULL);
        int32_t x;
        read(client_fd, &x, sizeof(x));
        x = ntohl(x);
        if (x == 0) {
            close(client_fd);
            break;
        }
        sum += x;
        close(client_fd);
    }
    printf("%lld\n", sum);
    close(listenfd);
    return 0;
}
