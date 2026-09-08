#ifndef NETWORK_H
#define NETWORK_H

/* POSIX TCP transport. The caller closes every successful connection. */
enum { MSG_QUIT = -1, MSG_DONE = -2, MSG_WIN = -3 };
typedef struct {
    int row;
    int col;
    int result;
} MoveMsg;

int startServer(int port);
int startClient(const char *ip, int port);
/* Return 1 for a complete message, 0 on disconnect or I/O failure. */
int sendMove(int socketFd, const MoveMsg *move);
int receiveMove(int socketFd, MoveMsg *move);
void closeConnection(int socketFd);
#endif
