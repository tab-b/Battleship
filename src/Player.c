#include <stdio.h>
#include "AttackBoard.h"
#include "ShipBoard.h"
#include "Player.h"
#include <string.h>

void initializePlayer(Player *player, char* name) {
    strncpy(player->name, name, sizeof(player->name - 1));
    player->name[sizeof(player->name) - 1] = '\0';
    initializeShipBoard(&player->shipboard);
    initializeAttackBoard(&player->atkboard);


    player->wins = 0;
    player->losses = 0;
}
int shipPlacementSequence(Player* player);
int validateInput();
int attackSequence(Player* attacker, Player* defender, char row, int col);
int hasPlayerLost(Player* player);