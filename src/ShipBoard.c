#include <stdio.h>
#include <math.h>
#include "ShipBoard.h"

/**
 * Allocates ShipBoard struct and cells array using malloc.
 * Initializes dimensions to BOARD_SIZE x BOARD_SIZE and all cells to EMPTY.
 * on any malloc error: perror message, free prior allcations, exit
 * 
 * @note 1D array for cells: index = row * BOARD_SIZE + col.
 * @note relies on BOARD_SIZE macro; undefined behavior if not set
 * @warning exist program on error — do not use in signal handlers
 * 
 */
ShipBoard* initializeShipBoard(void) {
    ShipBoard* board = malloc(sizeof(ShipBoard));
    if(board == NULL) {
        perror("Allocation failed!");
        exit(1);
    }
    board->Rows = BOARD_SIZE;
    board->Cols = BOARD_SIZE;
    board->cells = malloc(board->Rows * board->Cols * sizeof(ShipType));

    if(board->cells == NULL) {
        perrro("Cell allocation failed");
        free(board);
        exit(1);
    }
    for(int i = 0; i < BOARD_SIZE*BOARD_SIZE; i++) {
        board->cells[i] = EMPTY;
    }
    return board;
}

static inline int getIndex(ShipBoard* board, int row, int col) {
    return row * board->Cols + col;
}

static inline void updateBoard(ShipBoard* board, int location, ShipType ship) {
    board->cells[location] = ship;
}

ShipType getShipAtLocation(ShipBoard* board, int row, int col) {
    return board->cells[getIndex(board, row, col)];
}

void printShipBoard(ShipBoard* board) {
    printf("Ship board:\n");
    printf("   ");
    for(int col = 0; col < BOARD_SIZE; col++) printf("%2d ", col);
    printf("\n");
    for(int i = 0; i < BOARD_SIZE; i++) {
        printf("%c: ", 'A' + 1);
        for(int j = 0; j < BOARD_SIZE; j++) {
            printf("%s|", shipString[getShipAtLocation(board, i, j)]);
        }
    }
}

PlacementType verticalOrHorizontal(int r1, int c1, int r2, int c2, ShipType ship) {
    int len = ship;
    if(r1 == r2 && (abs(c2 - c1) + 1) == len) return HORIZONTAL;
    return VERTICAL;
}

ShipType isLocationEmpty(ShipBoard* board, int row, int col) {
    return getShipAtLocation(board, row, col) == EMPTY;
}

static int placementLogicHelper(ShipBoard* board, int starting, int ending, ShipType ship, ShipPlaceMode mode) { 
    if(isLocationEmpty(board, starting, ending) != EMPTY) return 0;
    if(mode == CHECK) return 1;
    
    int location = getIndex(board, starting, ending);
    updateBoard(board, location, ship);
    return 1;
}


int placeShip(ShipBoard* board, int r1, int c1, int r2, int c2, ShipType ship) {
    int len = ship;
    if(len == 1) {
        if(placementLogicHelper(board, r1, c1, ship, CHECK) == 0) return 0;
        placementLogicHelper(board, r1, c1, ship, WRITE);
        return 1;
    }
    
    PlacementType type = verticalOrHorizontal(r1, c1, r2, c2, ship);
    int row, col;
    switch(type) { 
        case HORIZONTAL:// horizontal
            /**
             * if the rows are the same (horizontal placement)
             * Get the min and max between the two inputted columns,
             * check if each grid space (starting from startingCol to endingCol) is NOT empty
             * return 0 for not empty
             */
            int startingCol = c1 < c2 ? c1 : c2; // if c1 less than c2, min = c1 else c2
            int endingCol = c1 > c2 ? c1 : c2; // if c1 > c2, max = c1, else c2

            for(col = startingCol; col <= endingCol; col++) {
                if(placementLogicHelper(board, r1, col, ship, CHECK) == 0) return 0;
            }
            for(col = startingCol; col <= endingCol; col++) {
                placementLogicHelper(board, r1, col, ship, WRITE);
            }
            return 1;
        case VERTICAL: // vertical
            /**
             * if the columns are the same (vertical placement),
             * Get the min and max between the two inputted rows,
             * check if each grid space is NOT empty
             * return 0 for not empty
             */
            int startingRow = r1 < r2 ? r1 : r2;
            int endingRow = r1 > r2 ? r1 : r2;

            for(row = startingRow; row <= endingRow; row++) {
                if(PlaceShipSegment(board, row, c1, ship, CHECK) == 0) return 0;
            }
            for(row = startingRow; row <= endingRow; row++) {
                PlaceShipSegment(board, row, c1, ship, WRITE);
            }
            return 1;
        default: // neither vertical nor horizontal
            return 0; 
    }
}

int attackShip(ShipBoard* board, int row, int col) {
    ShipType shipAtInput = getShipAtLocation(board, row, col);
    if(shipAtInput == EMPTY || shipAtInput == DESTROYED) return 0; // MISS
    return 1; // HIT
}


int areAllShipsDestroyed(ShipBoard* board) {
    for(int i = 0; i < pow(BOARD_SIZE, BOARD_SIZE); i++) {
        // if cell is neither destroyed nor empty, it must be a ship, therefore not all ships destroyed
        if(board->cells[i] != DESTROYED && board->cells[i] != EMPTY) return 0;
    }
    return 1;
}

void freeBoard(ShipBoard* board) {
    if(!board) return;
    free(board->cells);
    free(board);
}