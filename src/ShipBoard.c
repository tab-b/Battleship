#include <stdio.h>
#include <math.h>
#include "ShipBoard.h"

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

PlacementType verticalOrHorizontal(int r1, int c1, int r2, int c2) {
    if(r1 == r2) return HORIZONTAL;
    return VERTICAL;
}

ShipType isLocationEmpty(ShipBoard* board, int row, int col) {
    return getShipAtLocation(board, row, col) == EMPTY;
}

int placementLogicHelper(ShipBoard* board, int starting, int ending, ShipType ship) { 
    if(isLocationEmpty(board, starting, ending) != EMPTY) return 0;
    int location = getIndex(board, starting, ending);
    updateBoard(board, location, ship);
    return 1;
}

int placeShip(ShipBoard* board, int r1, int c1, int r2, int c2, ShipType ship) {
    PlacementType type = verticalOrHorizontal(r1, c1, r2, c2);
    int location;
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
            while(startingCol <= endingCol) {
                if(placementLogicHelper(board, startingCol, endingCol, ship) == 0) return 0;
                startingCol++;
            }
        case VERTICAL: // vertical
            /**
             * if the columns are the same (vertical placement),
             * Get the min and max between the two inputted rows,
             * check if each grid space is NOT empty
             * return 0 for not empty
             */
            int startingRow = r1 < r2 ? r1 : r2;
            int endingRow = r1 > r2 ? r1 : r2;
            while(startingCol <= endingCol) {
                if(placementLogicHelper(board, startingRow, endingRow, ship) == 0) return 0;
                startingRow++;
            }
        default: // neither vertical or horizontal
            return 0; 
    }
}

int areAllShipsDestroyed(ShipBoard* board) {
    int count = 0;
    for(int i = 0; i < pow(BOARD_SIZE, BOARD_SIZE); i++) {
        if(board->cells[i] == DESTROYED) count += 1;
    }
    return count == 15;
}