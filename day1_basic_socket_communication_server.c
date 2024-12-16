#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdbool.h>

int main() {
    int lfd = socket(AF_INET, SOCK_STREAM, 0);
    if (lfd == -1) {
        perror("socket");
        exit(0);
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(10000);
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(lfd, (struct sockaddr*) &addr, sizeof(addr)) == -1) {
        perror("bind");
        close(lfd);
        exit(0);
    }

    if (listen(lfd, 128) == -1) {
        perror("listen");
        close(lfd);
        exit(0);
    }

    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr); 
    int cfd = accept(lfd, (struct sockaddr*) &client_addr, &client_len);
    if (cfd == -1) {
        perror("accpet");
        close(lfd);
        exit(0);
    }

    char ip[20] = {0};
    printf("ip: %s, port: %d\n", inet_ntop(AF_INET, &client_addr.sin_addr, ip, sizeof(ip)), ntohs(client_addr.sin_port));

    while (true) {
        char buf[1000];
        memset(buf, 0, sizeof(buf));

        int len = read(cfd, buf, sizeof(buf));
        if (len == -1) {
            perror("read");
            break;
        } else if (len == 0 || strcmp(buf, "disconnect\n") == 0) {
            printf("client disconnected\n");
            break;
        } else {
            printf("client said: %s\n", buf);
            write(cfd, "message accpeted\n", 18);
        }
    }

    close(cfd);
    close(lfd);
    return 0;
}
