#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdbool.h>

int main() {
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1) {
        perror("socket");
        exit(0);
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(10000);
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr.s_addr);

    if (connect(fd, (struct sockaddr*) &addr, sizeof(addr)) == -1) {
        perror("connect");
        close(fd);
        exit(0);
    }

    int i = 0;
    while (true) {
        char buf[1000];
        memset(buf, 0, sizeof(buf));

        if (i <= 10) {
            sprintf(buf, "hello server %d!\n", i++);
        } else {
            sprintf(buf, "disconnect\n");
        }

        if (write(fd, buf, strlen(buf)) == -1) {
            perror("write");
            break;
        }

        int len = read(fd, buf, sizeof(buf));
        if (len == -1) {
            perror("read");
            break;
        } else if (len == 0) {
            printf("server disconnected\n");
            break;
        } else {
            printf("server said: %s\n", buf);
        }
    }

    close(fd);
    return 0;
}
