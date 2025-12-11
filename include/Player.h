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

void initializePlayer(Player* player, char* name);
int shipPlacementSequence(Player* player);
int attackSequence(Player* attacker, Player* defender, char row, int col);
int hasPlayerLost(Player* player);
#endif