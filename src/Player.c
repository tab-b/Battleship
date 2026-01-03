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

int hasPlayerLost(Player* player) {
    return areAllShipsDestroyed(&player->shipboard);
}

void generateValidRandomLocation(Player* computer, enum ShipType shipType, int *out_row1, int *out_col1, int *out_row2, int *out_col2) {
    int shipSize = shipType;
    int verticalOrHorizontal = rand() % 2; // 0 = horizontal, 1 = vertical
    int row1;
    int col1;
    int row2;
    int col2;
    if(verticalOrHorizontal == 0) { // horizontal 
        // generate locations as long as said location is occupied (invalid)
        row1 = row2 = rand() % 10; // generate random # from 0 (representing row A) to 9 (representing row J)
        do {
            col1 = rand() % (10 - shipSize + 1); // columns 0 to size of said ship
            col2 = shipSize - 1 + col1; // col2 must be size of ship minus 1 + col1 to get correction location of the correct size
        } while(checkRangeIfEmpty(&computer->shipboard, row1, col1, row2, col2) == 0); 
    } else { // vertical
        col1 = col2 = rand() % 10;
        do {
            // generate radnom number from 0 to size of ship - 1
            row1 = rand() % (10 - shipSize + 1);
            row2 = shipSize - 1 + row1;
        } while(checkRangeIfEmpty(&computer->shipboard, row1, col1, row2, col2) == 0);
    }
    *out_row1 = row1;
    *out_col1 = col1;
    *out_row2 = row2;
    *out_col2 = col2;
}


void autoPlacement(Player* computer) {
    int row1;
    int col1;
    int row2;
    int col2;
    for(int i = 0; i < 5; i++) {
        generateValidRandomLocation(computer, ships[i], row1, col1, row2, col2);
        placeShip(&computer->shipboard, row1, col1, row2, col2, ships[i]);
    }
}