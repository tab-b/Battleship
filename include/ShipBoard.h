#ifndef BOARD_H
#define BOARD_H
#include <stddef.h>

#define BOARD_SIZE 10

typdef enum {
    DESTROYED = 6, // X
    EMPTY = 0,
    DESTROYER = 1, // DT
    SUBMARINE = 2, // SB
    CRUISER = 3, // CR
    BATTLESHIP = 4, // BP
    CARRIER = 5, // CA
} ShipType;

typedef struct {
    ShipType shipboard[BOARD_SIZE][BOARD_SIZE];
} ShipBoard;


void initializeShipBoard(ShipBoard* shipboard);
void printShipBoard(ShipBoard* shipboard);
int placeShip(ShipBoard* shipboard, char row, int col, ShipType ship);
int areAllShipsDestroyed(ShipBoard* shipboard);
#endif



