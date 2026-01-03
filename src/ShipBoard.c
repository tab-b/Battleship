#include <stdio.h>
#include <math.h>
#include "ShipBoard.h"

ShipType ships[] = {DESTROYER, SUBMARINE, CRUISER, BATTLESHIP, CARRIER};
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
void initializeShipBoard(ShipBoard* board) {
    board = malloc(sizeof(ShipBoard));
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
        printf("%c: ", 'A' + i);
        for(int j = 0; j < BOARD_SIZE; j++) {
            printf("%s|", shipString[getShipAtLocation(board, i, j)]);
        }
    }
}

PlacementType verticalOrHorizontal(int r1, int c1, int r2, int c2, ShipType ship) {
    int len = ship;
    if(r1 == r2 && (abs(c2 - c1) + 1) == len) return HORIZONTAL;
    else if(c1 == c2 && (abs(r2 - r1) + 1) == len) return VERTICAL;
    return -1;
}

ShipType isLocationEmpty(ShipBoard* board, int row, int col) {
    return getShipAtLocation(board, row, col) == EMPTY;
}
/**
 * @brief Check if range of coordinates does not have a ship placed there
 * 
 * @param board player's board
 * @param r1 row 1
 * @param c1 col 1
 * @param r2 row 2
 * @param c2 col 2
 * @return int 1 if empty, 0 if any 1 of the coordinates in the range is not empty (aka a ship is already placed there)
 */
int checkRangeIfEmpty(ShipBoard* board, int r1, int c1, int r2, int c2) {
    if(r1 == r2) { // horizontal
        /**
         * if the rows are the same (horizontal placement)
         * Get the min and max between the two inputted columns,
         * check if each grid space (starting from minimumColol to maximumColol) is NOT empty
         * return 0 for not empty
         */
        int minimumCol = c1 < c2 ? c1 : c2; // if c1 less than c2, min = c1 else c2
        int maximumCol = c1 > c2 ? c1 : c2; // if c1 > c2, max = c1, else c2
        for(int c = minimumCol; c <= maximumCol; c++)
            if(isLocationEmpty(board, r1, c) != EMPTY) return 0;
    } else if(c1 == c2) { // vertical
        /**
         * if the columns are the same (vertical placement),
         * Get the min and max between the two inputted rows,
         * check if each grid space is NOT empty
         * return 0 for not empty
         */
        int minimumRow = r1 < r2 ? r1 : r2;
        int maximumRow = r1 > r2 ? r1 : r2;
        for(int r = minimumRow; r <= maximumRow; r++)
            if(isLocationEmpty(board, c1, r) != EMPTY) return 0;
    } else {
        return 0; // not straight
    }
    return 1;
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
                if(placementLogicHelper(board, row, c1, ship, CHECK) == 0) return 0;
            }
            for(row = startingRow; row <= endingRow; row++) {
                placementLogicHelper(board, row, c1, ship, WRITE);
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