#ifndef GAME_H
#define GAME_H
#include "ui.h"

GameStatus playSinglePlayer(void);
GameStatus playMultiplayer(int socketFd, int isServer);
#endif
