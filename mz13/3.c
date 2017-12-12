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
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serv_addr =
    {
        .sin_family = AF_INET,
        .sin_port = htons(port),
        .sin_addr.s_addr = htonl(INADDR_ANY)
    };
    bind(listenfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    listen(listenfd, 0);
    int clifd;
    long long sum = 0;
    while (1) {
        clifd = accept(listenfd, NULL, NULL);
        int x;
        read(clifd, &x, sizeof(x));
        x = htonl(x);
        if (x == 0) {
            break;
        }
        sum += x;
        close(clifd);
    }
    printf("%lld\n", sum);
    return 0;
}
