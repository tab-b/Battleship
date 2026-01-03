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

enum ShipType ships[] = {DESTROYER, SUBMARINE, CRUISER, BATTLESHIP, CARRIER};

typedef enum {
    HORIZONTAL,
    VERTICAL,
} PlacementType;

typedef enum {
    CHECK,
    WRITE
} ShipPlaceMode;

typedef struct {
    int Rows;
    int Cols;
    ShipType *cells;
} ShipBoard;


/**
 * @brief Allocates and intitializes a new ShipBoard for the game
 * Creates a ShipBoard with fixed dimensions (BOARD_SIZE x BOARD_SIZE),
 * allocates a 1D array of ShipType cells, and sets all cells to EMPTY.
 * Exits the program on allocation failure and using perror and free where possible.
 * 
 * @return ShipBoard* Pointer to the the initialized ShipBoard, or program terminates on error.
 * @note Caller must free the returned board using a corresponding free function
 * @note BOARD_SIZE must be defined before use
 */
void initializeShipBoard(ShipBoard* board);
ShipType getShipAtLocation(ShipBoard* board, int row, int col);
void printShipBoard(ShipBoard* board);
int placeShip(ShipBoard* shipboard,  int r1, int c1, int r2, int c2, ShipType ship);
int attackShip(ShipBoard* board, int row, int col);
int areAllShipsDestroyed(ShipBoard* board);
int checkRangeIfEmpty(ShipBoard* board, int r1, int c1, int r2, int c2);
void freeBoard(ShipBoard* board);
#endif



