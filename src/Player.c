#include <stdio.h>
#include <stdlib.h>
#include "Player.h"

void initializePlayer(Player *player, const char *name) {
    snprintf(player->name, sizeof(player->name), "%s", name);
    initializeShipBoard(&player->shipboard);
    initializeAttackBoard(&player->atkboard);
    player->wins = player->losses = 0;
}

void freePlayer(Player *player) {
    if(!player) return;
    freeShipBoard(&player->shipboard);
    freeAttackBoard(&player->atkboard);
}

int hasPlayerLost(const Player *player) {
    return areAllShipsDestroyed(&player->shipboard);
}

void autoPlacement(Player *player) {
    for(int i = 0; i < SHIP_COUNT; ++i) {
        int r1, c1, r2, c2;
        do {
            int vertical = rand() % 2;
            r1 = rand() % (BOARD_SIZE - (vertical ? (int)ships[i] - 1 : 0));
            c1 = rand() % (BOARD_SIZE - (vertical ? 0 : (int)ships[i] - 1));
            r2 = r1 + (vertical ? (int)ships[i] - 1 : 0);
            c2 = c1 + (vertical ? 0 : (int)ships[i] - 1);
        } while(!placeShip(&player->shipboard, r1, c1, r2, c2, ships[i]));
    }
}

int attackPlayer(Player *attacker, Player *defender, int row, int col) {
    if(row < 0 || row >= BOARD_SIZE || col < 0 || col >= BOARD_SIZE) return -1;
    if(wasAttackAlreadyAttempted(&attacker->atkboard, row, col)) return -1;
    int hit = attackShip(&defender->shipboard, row, col);
    updateAttackBoard(&attacker->atkboard, row, col, hit ? HIT : MISS);
    return hit;
}

int computerAttack(Player *computer, Player *defender, int *row, int *col) {
    /* Choose among remaining cells, including when only one remains. */
    int remaining = 0;
    for(int r = 0; r < BOARD_SIZE; ++r)
        for(int c = 0; c < BOARD_SIZE; ++c)
            if(!wasAttackAlreadyAttempted(&computer->atkboard, r, c)) ++remaining;
    if(!remaining) return -1;
    int choice = rand() % remaining;
    for(int r = 0; r < BOARD_SIZE; ++r) {
        for(int c = 0; c < BOARD_SIZE; ++c) {
            if(!wasAttackAlreadyAttempted(&computer->atkboard, r, c) && choice-- == 0) {
                *row = r;
                *col = c;
                return attackPlayer(computer, defender, r, c);
            }
        }
    }
    return -1;
}
