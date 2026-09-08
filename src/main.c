#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Game.h"
#include "Network.h"

static int parsePort(const char *text) {
    char *end;
    errno = 0;
    long port = strtol(text, &end, 10);
    if(errno || end == text || *end || port < 1 || port > 65535) return -1;
    return (int)port;
}

int main(int argc, char *argv[]) {
    srand((unsigned int)time(NULL));
    GameStatus status;
    if(argc == 1) {
        puts("Starting single player game against computer.");
        status = playSinglePlayer();
    } else if(argc == 2 || argc == 3) {
        int port = parsePort(argv[argc - 1]);
        if(port < 0) {
            fputs("Port must be a number from 1 to 65535.\n", stderr);
            return EXIT_FAILURE;
        }
        int connection = argc == 2 ? startServer(port) : startClient(argv[1], port);
        if(connection < 0) return EXIT_FAILURE;
        status = playMultiplayer(connection, argc == 2);
        closeConnection(connection);
    } else {
        displayMainMenu(argv[0]);
        return EXIT_FAILURE;
    }
    if(status == GAME_COMPLETE) puts("Game complete! Thank you for playing!");
    else if(status == INPUT_QUIT) puts("Game ended before completion.");
    else fputs("Game ended with an error.\n", stderr);
    return status == INPUT_ERROR ? EXIT_FAILURE : EXIT_SUCCESS;
}
