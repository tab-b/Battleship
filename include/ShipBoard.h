#ifndef SHIP_BOARD_H
#define SHIP_BOARD_H
#include "Board.h"

typedef enum {
    EMPTY = 0, DESTROYER = 1, SUBMARINE = 2, CRUISER = 3,
    BATTLESHIP = 4, CARRIER = 5, DESTROYED = 6
} ShipType;

typedef struct {
    int Rows;
    int Cols;
    ShipType *cells;
} ShipBoard;

extern const ShipType ships[SHIP_COUNT];
/* The caller owns the struct. Only cells are allocated and freed. */
void initializeShipBoard(ShipBoard *board);
void freeShipBoard(ShipBoard *board);
/* Getter coordinates must be inside the board. */
ShipType getShipAtLocation(const ShipBoard *board, int row, int col);
int checkRangeIfEmpty(const ShipBoard *board, int r1, int c1, int r2, int c2);
/* Returns 1 on success, 0 for invalid bounds, length, or overlap. */
int placeShip(ShipBoard *board, int r1, int c1, int r2, int c2, ShipType ship);
/* Marks hits DESTROYED. Returns 1 for hit, 0 for miss, -1 for bad bounds. */
int attackShip(ShipBoard *board, int row, int col);
int areAllShipsDestroyed(const ShipBoard *board);
#endif
