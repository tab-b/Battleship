#ifndef BOARD_H
#define BOARD_H
#include <stddef.h>

#define BOARD_SIZE 10

const char *shipString[] = {"  ", "DT", "SB", "CR", "BP", "CA", "X "};

typedef enum {
    DESTROYED = 6, // X
    EMPTY = 0,
    DESTROYER = 1, // DT
    SUBMARINE = 2, // SB
    CRUISER = 3, // CR
    BATTLESHIP = 4, // BP
    CARRIER = 5, // CA
} ShipType;

typedef enum {
    HORIZONTAL,
    VERTICAL,
} PlacementType;

typedef struct {
    int Rows;
    int Cols;
    ShipType *cells;
} ShipBoard;



ShipBoard* initializeShipBoard(void);
ShipType getShipAtLocation(ShipBoard* board, int row, int col);
void printShipBoard(ShipBoard* board);
// int isPlacementEmpty(ShipBoard* board, int r1, int c1, int r2, int c2);
int placeShip(ShipBoard* shipboard, int r1, int c1, int r2, int c2, ShipType ship);
int areAllShipsDestroyed(ShipBoard* board);
#endif



