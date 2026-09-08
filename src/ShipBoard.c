#include <stdio.h>
#include <stdlib.h>
#include "ShipBoard.h"

const ShipType ships[SHIP_COUNT] = {
    DESTROYER, SUBMARINE, CRUISER, BATTLESHIP, CARRIER
};

static int inBounds(const ShipBoard *board, int row, int col) {
    return row >= 0 && row < board->Rows && col >= 0 && col < board->Cols;
}

void initializeShipBoard(ShipBoard *board) {
    board->Rows = board->Cols = BOARD_SIZE;
    board->cells = calloc((size_t)board->Rows * board->Cols, sizeof(*board->cells));
    if(!board->cells) {
        perror("Ship board allocation failed");
        exit(EXIT_FAILURE);
    }
}

void freeShipBoard(ShipBoard *board) {
    if(!board) return;
    free(board->cells);
    board->cells = NULL;
    board->Rows = board->Cols = 0;
}

ShipType getShipAtLocation(const ShipBoard *board, int row, int col) {
    return board->cells[row * board->Cols + col];
}

int checkRangeIfEmpty(const ShipBoard *board, int r1, int c1, int r2, int c2) {
    if(!inBounds(board, r1, c1) || !inBounds(board, r2, c2)) return 0;
    if(r1 != r2 && c1 != c2) return 0;
    int dr = (r2 > r1) - (r2 < r1);
    int dc = (c2 > c1) - (c2 < c1);
    for(;;) {
        if(getShipAtLocation(board, r1, c1) != EMPTY) return 0;
        if(r1 == r2 && c1 == c2) return 1;
        r1 += dr;
        c1 += dc;
    }
}

int placeShip(ShipBoard *board, int r1, int c1, int r2, int c2, ShipType ship) {
    if(ship < DESTROYER || ship > CARRIER) return 0;
    if(!checkRangeIfEmpty(board, r1, c1, r2, c2)) return 0;
    if(abs(r2 - r1) + abs(c2 - c1) + 1 != (int)ship) return 0;
    int dr = (r2 > r1) - (r2 < r1);
    int dc = (c2 > c1) - (c2 < c1);
    for(;;) {
        board->cells[r1 * board->Cols + c1] = ship;
        if(r1 == r2 && c1 == c2) return 1;
        r1 += dr;
        c1 += dc;
    }
}

int attackShip(ShipBoard *board, int row, int col) {
    if(!inBounds(board, row, col)) return -1;
    ShipType ship = getShipAtLocation(board, row, col);
    if(ship == EMPTY || ship == DESTROYED) return 0;
    board->cells[row * board->Cols + col] = DESTROYED;
    return 1;
}

int areAllShipsDestroyed(const ShipBoard *board) {
    for(int i = 0; i < board->Rows * board->Cols; ++i)
        if(board->cells[i] != EMPTY && board->cells[i] != DESTROYED) return 0;
    return 1;
}
