#ifndef PLAYER_H
#define PLAYER_H
#include "AttackBoard.h"
#include "ShipBoard.h"

typedef struct {
    char name[50];
    ShipBoard shipboard;
    AttackBoard atkboard;
    int wins;
    int losses;
} Player;

void initializePlayer(Player *player, const char *name);
void freePlayer(Player *player);
int hasPlayerLost(const Player *player);
/* Call on an empty board to place one of each ship. */
void autoPlacement(Player *player);
/* Returns -1 for invalid/repeated attacks, otherwise 0 (miss) or 1 (hit). */
int attackPlayer(Player *attacker, Player *defender, int row, int col);
int computerAttack(Player *computer, Player *defender, int *row, int *col);
#endif
