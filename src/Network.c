#include <errno.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "Network.h"

int startServer(int port) {
    signal(SIGPIPE, SIG_IGN);
    int listener = socket(AF_INET, SOCK_STREAM, 0);
    if(listener < 0) { perror("socket"); return -1; }
    int reuse = 1;
    setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
    struct sockaddr_in address = {0};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons((unsigned short)port);
    if(bind(listener, (struct sockaddr *)&address, sizeof(address)) < 0 ||
        listen(listener, 1) < 0) {
        perror("listen/bind");
        close(listener);
        return -1;
    }
    printf("Waiting for a player on port %d...\n", port);
    fflush(stdout);
    int connection;
    do {
        connection = accept(listener, NULL, NULL);
    } while(connection < 0 && errno == EINTR);
    if(connection < 0) perror("accept");
    close(listener);
    return connection;
}

int startClient(const char *ip, int port) {
    signal(SIGPIPE, SIG_IGN);
    int connection = socket(AF_INET, SOCK_STREAM, 0);
    if(connection < 0) { perror("socket"); return -1; }
    struct sockaddr_in address = {0};
    address.sin_family = AF_INET;
    address.sin_port = htons((unsigned short)port);
    if(inet_pton(AF_INET, ip, &address.sin_addr) != 1) {
        fprintf(stderr, "Invalid IPv4 address: %s\n", ip);
        close(connection);
        return -1;
    }
    if(connect(connection, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("connect");
        close(connection);
        return -1;
    }
    return connection;
}

int sendMove(int socketFd, const MoveMsg *move) {
    /* Messages use native int sizes and byte order on both peers. */
    const unsigned char *bytes = (const unsigned char *)move;
    size_t sent = 0;
    while(sent < sizeof(*move)) {
        ssize_t count = send(socketFd, bytes + sent, sizeof(*move) - sent, 0);
        if(count < 0 && errno == EINTR) continue;
        if(count <= 0) return 0;
        sent += (size_t)count;
    }
    return 1;
}

int receiveMove(int socketFd, MoveMsg *move) {
    unsigned char *bytes = (unsigned char *)move;
    size_t received = 0;
    while(received < sizeof(*move)) {
        ssize_t count = recv(socketFd, bytes + received, sizeof(*move) - received, 0);
        if(count < 0 && errno == EINTR) continue;
        if(count <= 0) return 0;
        received += (size_t)count;
    }
    return 1;
}

void closeConnection(int socketFd) {
    if(socketFd >= 0) close(socketFd);
}
