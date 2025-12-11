#include <stdio.h>
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

int isPlacementEmpty(ShipBoard* board, int r1, int c1, int r2, int c2) {
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
            if(getShipAtLocation(board, r1, c) != EMPTY) return 0;
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
            if(getShipAtLocation(board, r, c1) != EMPTY) return 0;
    } else {
        return 0; // not straight
    }
    return 1;
}